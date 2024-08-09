# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#   NScD Oak Ridge National Laboratory, European Spallation Source,
#   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
# SPDX - License - Identifier: GPL - 3.0 +
# pylint: disable=invalid-name,redefined-builtin
import mantid.simpleapi as s_api
from mantid import config, logger
from mantid.api import MatrixWorkspace
from typing import Tuple

import math
import re
import numpy as np


def get_run_number(ws_name):
    """
    Gets the run number for a given workspace.

    Attempts to get from logs and falls back to parsing the workspace name for
    something that looks like a run number.

    @param ws_name Name of workspace
    @return Parsed run number
    """

    workspace = s_api.mtd[ws_name]
    run_number = str(workspace.getRunNumber())
    if run_number == "0":
        # Attempt to parse run number off of name
        match = re.match(r"([a-zA-Z]+)([0-9]+)", ws_name)
        if match:
            run_number = match.group(2)
        else:
            # attempt reading from the logs (ILL)
            run = workspace.getRun()
            if run.hasProperty("run_number"):
                log = run.getLogData("run_number").value
                run_number = log.split(",")[0]
            else:
                raise RuntimeError("Could not find run number associated with workspace.")

    return run_number


def get_instrument_and_run(ws_name):
    """
    Get the instrument name and run number from a workspace.

    @param ws_name - name of the workspace
    @return tuple of form (instrument, run number)
    """

    run_number = get_run_number(ws_name)

    instrument = s_api.mtd[ws_name].getInstrument().getName()
    if instrument != "":
        for facility in config.getFacilities():
            try:
                instrument = facility.instrument(instrument).filePrefix(int(run_number))
                instrument = instrument.lower()
                break
            except RuntimeError:
                continue

    return instrument, run_number


def get_workspace_name_prefix(wsname):
    """
    Returns a string of the form '<ins><run>_<analyser><refl>_' on which
    all of our other naming conventions are built. The workspace is used to get the
    instrument parameters.
    """
    if wsname == "":
        return ""

    workspace = s_api.mtd[wsname]
    facility = config["default.facility"]

    ws_run = workspace.getRun()
    if "facility" in ws_run:
        facility = ws_run.getLogData("facility").value

    instrument, run_number = get_instrument_and_run(wsname)
    if facility == "ILL":
        run_name = instrument + "_" + run_number
    else:
        run_name = instrument + run_number

    try:
        analyser = workspace.getInstrument().getStringParameter("analyser")[0]
        reflection = workspace.getInstrument().getStringParameter("reflection")[0]
    except IndexError:
        analyser = ""
        reflection = ""

    prefix = run_name + "_" + analyser + reflection

    if len(analyser + reflection) > 0:
        prefix += "_"

    return prefix


def get_efixed(workspace):
    if isinstance(workspace, str):
        inst = s_api.mtd[workspace].getInstrument()
    else:
        inst = workspace.getInstrument()

    if inst.hasParameter("Efixed"):
        return inst.getNumberParameter("EFixed")[0]

    if inst.hasParameter("analyser"):
        analyser_name = inst.getStringParameter("analyser")[0]
        analyser_comp = inst.getComponentByName(analyser_name)

        if analyser_comp is not None and analyser_comp.hasParameter("Efixed"):
            return analyser_comp.getNumberParameter("EFixed")[0]

    if efixed_log := _try_get_sample_log(workspace, "EFixed"):
        return float(efixed_log)
    # For Direct data, we can use "Ei" in the sample logs as EFixed
    if ei_log := _try_get_sample_log(workspace, "Ei"):
        return float(ei_log)

    raise ValueError("No Efixed parameter found")


def get_two_theta_angles(inWS):
    if isinstance(inWS, str):
        ws = s_api.mtd[inWS]
    else:
        ws = inWS
    num_hist = ws.getNumberHistograms()  # get no. of histograms/groups
    source_pos = ws.getInstrument().getSource().getPos()
    sample_pos = ws.getInstrument().getSample().getPos()
    beam_pos = sample_pos - source_pos
    angles = []  # will be list of angles
    for index in range(0, num_hist):
        detector = ws.getDetector(index)  # get index
        two_theta = detector.getTwoTheta(sample_pos, beam_pos) * 180.0 / math.pi  # calc angle
        angles.append(two_theta)  # add angle
    return angles


def get_theta_q(ws_in):
    """
    Returns the theta and elastic Q for each spectrum in a given workspace.

    @param ws Workspace to get theta and Q for
    @returns A tuple containing a list of theta values and a list of Q values
    """
    if isinstance(ws_in, str):
        ws = s_api.mtd[ws_in]
    else:
        ws = ws_in
    e_fixed = get_efixed(ws)
    wavelas = math.sqrt(81.787 / e_fixed)  # Elastic wavelength
    k0 = 4.0 * math.pi / wavelas

    axis = ws.getAxis(1)

    # If axis is in spec number need to retrieve angles and calculate Q
    if axis.isSpectra():
        theta = np.array(get_two_theta_angles(ws))
        q = k0 * np.sin(0.5 * np.radians(theta))

    # If axis is in Q need to calculate back to angles and just return axis values
    elif axis.isNumeric() and axis.getUnit().unitID() == "MomentumTransfer":
        q_bin_edge = axis.extractValues()
        q = list()
        for i in range(1, len(q_bin_edge)):
            q_centre = ((q_bin_edge[i] - q_bin_edge[i - 1]) / 2) + q_bin_edge[i - 1]
            q.append(q_centre)
        np_q = np.array(q)
        theta = 2.0 * np.degrees(np.arcsin(np_q / k0))

    # Out of options here
    else:
        raise RuntimeError("Cannot get theta and Q for workspace %s" % ws)

    return theta, q


def extract_float(data_string):
    """
    Extract float values from an ASCII string
    """
    values = data_string.split()
    values = [float(v) for v in values]
    return values


def extract_int(data_string):
    """
    Extract int values from an ASCII string
    """
    values = data_string.split()
    values = [int(v) for v in values]
    return values


def pad_array(inarray, nfixed):
    """
    Pad a list to specified size.
    """
    npt = len(inarray)
    padding = nfixed - npt
    outarray = []
    outarray.extend(inarray)
    outarray += [0] * padding
    return outarray


def _check_analysers_are_equal(in1WS, in2WS):
    """
    Check workspaces have identical analysers and reflections

    Args:
      @param in1WS - first 2D workspace
      @param in2WS - second 2D workspace

    Returns:
      @return None

    Raises:
      @exception ValueError - workspaces have different analysers
      @exception ValueError - workspaces have different reflections
    """
    ws1 = s_api.mtd[in1WS]
    try:
        analyser_1 = ws1.getInstrument().getStringParameter("analyser")[0]
        reflection_1 = ws1.getInstrument().getStringParameter("reflection")[0]
    except IndexError:
        # Ignore this check if an analyser or reflection cannot be found
        return
    ws2 = s_api.mtd[in2WS]
    try:
        analyser_2 = ws2.getInstrument().getStringParameter("analyser")[0]
        reflection_2 = ws2.getInstrument().getStringParameter("reflection")[0]
    except:
        # Ignore this check if an analyser or reflection cannot be found
        return

    if analyser_1 != analyser_2:
        raise ValueError("Workspace %s and %s have different analysers" % (ws1, ws2))
    elif reflection_1 != reflection_2:
        raise ValueError("Workspace %s and %s have different reflections" % (ws1, ws2))
    else:
        logger.information("Analyser is %s, reflection %s" % (analyser_1, reflection_1))


def _get_sample_log(workspace, log_name):
    table = s_api.CreateLogPropertyTable(workspace, log_name, EnableLogging=False, StoreInADS=False)
    log_value = table.cell(0, 0) if table else None
    return log_value


def _try_get_sample_log(workspace, log_name):
    messages = s_api.CheckForSampleLogs(workspace, log_name, EnableLogging=False)
    return _get_sample_log(workspace, log_name) if not messages else None


def _check_e_fixed_are_equal(workspace1, workspace2):
    if get_efixed(workspace1) != get_efixed(workspace2):
        raise ValueError("Workspaces {str(workspace1)} and {str(workspace2)} have a different EFixed.")


def _is_technique_direct(workspace):
    return _try_get_sample_log(workspace, "deltaE-mode") == "Direct"


def check_analysers_or_e_fixed(workspace1, workspace2):
    """
    Check that the workspaces have EFixed if the technique is direct, otherwise check that the analysers and
    reflections are identical
    """
    if _is_technique_direct(workspace1) or _is_technique_direct(workspace2):
        logger.warning("Could not find an analyser for the input workspaces because the energy mode is Direct")
        _check_e_fixed_are_equal(workspace1, workspace2)
    else:
        _check_analysers_are_equal(workspace1, workspace2)


def check_hist_zero(inWS):
    """
    Retrieves basic info on a workspace

    Checks the workspace is not empty, then returns the number of histogram and
    the number of X-points, which is the number of bin boundaries minus one

    Args:
      @param inWS  2D workspace

    Returns:
      @return num_hist - number of histograms in the workspace
      @return ntc - number of X-points in the first histogram, which is the number of bin
           boundaries minus one. It is assumed all histograms have the same
           number of X-points.

    Raises:
      @exception ValueError - Worskpace has no histograms
    """
    num_hist = s_api.mtd[inWS].getNumberHistograms()  # no. of hist/groups in WS
    if num_hist == 0:
        raise ValueError("Workspace " + inWS + " has NO histograms")
    x_in = s_api.mtd[inWS].readX(0)
    ntc = len(x_in) - 1  # no. points from length of x array
    if ntc == 0:
        raise ValueError("Workspace " + inWS + " has NO points")
    return num_hist, ntc


def check_dimensions_equal(in1WS, name1, in2WS, name2):
    """
    Check workspaces have the same number of histograms and bin boundaries

    Args:
      @param in1WS - first 2D workspace
      @param name1 - single-word descriptor of first 2D workspace
      @param in2WS - second 2D workspace
      @param name2 - single-word descriptor of second 2D workspace

    Returns:
      @return None

    Raises:
      Valuerror: number of histograms is different
      Valuerror: number of bin boundaries in the histograms is different
    """
    num_hist_1 = s_api.mtd[in1WS].getNumberHistograms()  # no. of hist/groups in WS1
    x_1 = s_api.mtd[in1WS].readX(0)
    x_len_1 = len(x_1)
    num_hist_2 = s_api.mtd[in2WS].getNumberHistograms()  # no. of hist/groups in WS2
    x_2 = s_api.mtd[in2WS].readX(0)
    x_len_2 = len(x_2)
    if num_hist_1 != num_hist_2:  # Check that no. groups are the same
        error_1 = "%s (%s) histograms (%d)" % (name1, in1WS, num_hist_1)
        error_2 = "%s (%s) histograms (%d)" % (name2, in2WS, num_hist_2)
        error = error_1 + " not = " + error_2
        raise ValueError(error)
    elif x_len_1 != x_len_2:
        error_1 = "%s (%s) array length (%d)" % (name1, in1WS, x_len_1)
        error_2 = "%s (%s) array length (%d)" % (name2, in2WS, x_len_2)
        error = error_1 + " not = " + error_2
        raise ValueError(error)


def check_x_range(x_range, range_type):
    if not ((len(x_range) == 2) or (len(x_range) == 4)):
        raise ValueError(range_type + " - Range must contain either 2 or 4 numbers")

    for lower, upper in zip(x_range[::2], x_range[1::2]):
        if math.fabs(lower) < 1e-5:
            raise ValueError("%s - input minimum (%f) is zero" % (range_type, lower))
        if math.fabs(upper) < 1e-5:
            raise ValueError("%s - input maximum (%f) is zero" % (range_type, upper))
        if upper < lower:
            raise ValueError("%s - input maximum (%f) < minimum (%f)" % (range_type, upper, lower))


def convert_to_elastic_q(input_ws, output_ws=None):
    """
    Helper function to convert the spectrum axis of a sample to ElasticQ.

    @param input_ws - the name of the workspace to convert from
    @param output_ws - the name to call the converted workspace
    """

    if output_ws is None:
        output_ws = input_ws

    axis = s_api.mtd[input_ws].getAxis(1)
    if axis.isSpectra():
        e_fixed = get_efixed(input_ws)
        s_api.ConvertSpectrumAxis(input_ws, Target="ElasticQ", EMode="Indirect", EFixed=e_fixed, OutputWorkspace=output_ws)

    elif axis.isNumeric():
        # Check that units are Momentum Transfer
        if axis.getUnit().unitID() != "MomentumTransfer":
            raise RuntimeError("Input must have axis values of Q")

        s_api.CloneWorkspace(input_ws, OutputWorkspace=output_ws)

    else:
        raise RuntimeError("Input workspace must have either spectra or numeric axis.")


def transpose_fit_parameters_table(params_table, output_table=None):
    """
    Transpose the parameter table created from a multi domain Fit.

    This function will make the output consistent with PlotPeakByLogValue.
    @param params_table - the parameter table output from Fit.
    @param output_table - name to call the transposed table. If omitted,
            the output_table will be the same as the params_table
    """
    params_table = s_api.mtd[params_table]

    table_ws = "__tmp_table_ws"
    table_ws = s_api.CreateEmptyTableWorkspace(OutputWorkspace=table_ws)

    param_names = params_table.column(0)[:-1]  # -1 to remove cost function
    param_values = params_table.column(1)[:-1]
    param_errors = params_table.column(2)[:-1]

    # Find the number of parameters per function
    func_index = param_names[0].split(".")[0]
    num_params = 0
    for i, name in enumerate(param_names):
        if name.split(".")[0] != func_index:
            num_params = i
            break

    # Create columns with parameter names for headers
    column_names = [".".join(name.split(".")[1:]) for name in param_names[:num_params]]
    column_error_names = [name + "_Err" for name in column_names]
    column_names = list(zip(column_names, column_error_names))
    table_ws.addColumn("double", "axis-1")
    for name, error_name in column_names:
        table_ws.addColumn("double", name)
        table_ws.addColumn("double", error_name)

    # Output parameter values to table row
    for i in range(0, params_table.rowCount() - 1, num_params):
        row_values = param_values[i : i + num_params]
        row_errors = param_errors[i : i + num_params]
        row = [value for pair in zip(row_values, row_errors) for value in pair]
        row = [i / num_params] + row
        table_ws.addRow(row)

    if output_table is None:
        output_table = params_table.name()

    s_api.RenameWorkspace(table_ws.name(), OutputWorkspace=output_table)


def _first_non_zero(data):
    """
    Returns the index of the first non zero value in the list
    """
    for i in range(len(data)):
        if data[i] != 0:
            return i


def identify_non_zero_bin_range(workspace: MatrixWorkspace, workspace_index: int) -> Tuple[float]:
    """
    Identifies the bin range within which there is no trailing or leading zero values for a given workspace index.

    @param workspace: the workspace containing spectra with bins.
    @param workspace_index: the workspace index to identify the non-zero bin range within.
    @return a tuple of the first and last non-zero values in a spectrum
    """
    # Identify bin index of first and last non-zero y value
    y_data = workspace.readY(workspace_index)
    start_data_idx = _first_non_zero(y_data)
    end_data_idx = _first_non_zero(list(reversed(y_data)))
    # Assumes common bin boundaries for each spectra
    x_data = workspace.readX(0)
    return x_data[start_data_idx], x_data[len(x_data) - end_data_idx - 1]


def format_runs(runs, instrument_name):
    """
    :return: A list of runs prefixed with the given instrument name
    """
    run_list = []
    for run in runs:
        if "-" in run:
            a, b = run.split("-")
            run_list.extend(range(int(a), int(b) + 1))
        else:
            try:
                run_list.append(int(run))
            except:
                # run already has instrument eg 'osi1000'
                run_list.append(run)
    for idx, run in enumerate(run_list):
        if isinstance(run, int):
            run_list[idx] = instrument_name + str(run)
    return run_list
