# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#   NScD Oak Ridge National Laboratory, European Spallation Source,
#   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
# SPDX - License - Identifier: GPL - 3.0 +
from mantid.api import AlgorithmFactory, AnalysisDataService, WorkspaceFactory
from mantid.kernel import Logger, Property
from mantid.simpleapi import AbsorptionCorrection, DeleteWorkspace, Divide, Load, Multiply, \
    PaalmanPingsAbsorptionCorrection, PreprocessDetectorsToMD, SetSample, mtd
import numpy as np
import os

_EXTENSIONS_NXS = ["_event.nxs", ".nxs.h5"]


def _getBasename(filename):
    """
    Helper function to get the filename without the path or extension
    """
    if type(filename) == list:
        filename = filename[0]
    name = os.path.split(filename)[-1]
    for extension in _EXTENSIONS_NXS:
        name = name.replace(extension, '')
    return name


def calculate_absorption_correction(filename, abs_method, char_dict, sample_formula, mass_density,
                                    number_density=Property.EMPTY_DBL,
                                    container_shape="PAC06",
                                    num_wl_bins=1000,
                                    element_size=1):
    """The absorption correction is applied by (I_s - I_c*k*A_csc/A_cc)/A_ssc for pull Paalman-Ping

    If no cross-term then I_s/A_ss - I_c/A_cc

    Therefore this will return 2 workspace, one for correcting the
    sample (A_s) and one for the container (A_c) depending on the
    absorption method, that will be passed to _focusAndSum and
    therefore AlignAndFocusPowderFromFiles.

    If SampleOnly then

    A_s = A_ss
    A_c = None

    If SampleAndContainer then

    A_s = A_ss
    A_c = A_cc

    If FullPaalmanPings then
    A_s = A_ssc
    A_c = A_cc*A_ssc/A_csc

    This will then return (A_s, A_c)

    :param filename: File to be used for absorption correction
    :param abs_method: Type of absorption correction: None, SampleOnly, SampleAndContainer, FullPaalmanPings
    :param char_dict: Characterization dictionary (see PDDetermineCharacterization) for creating donor workspace
    :param sample_formula: Sample formula to specify the Material for absorption correction
    :param mass_density: Mass density of the sample to specify the Material for absorption correction
    :param number_density: Optional number density of sample to be added to the Material for absorption correction
    :param container_shape: Shape definition of container, such as PAC06.
    :param num_wl_bins: Number of bins for calculating wavelength
    :param element_size: Size of one side of the integration element cube in mm
    :return:
    """
    if abs_method == "None":
        return None, None

    material = {"ChemicalFormula": sample_formula,
                "SampleMassDensity": mass_density}

    if number_density != Property.EMPTY_DBL:
        material["SampleNumberDensity"] = number_density

    environment = {'Name': 'InAir', 'Container': container_shape}

    donorWS = create_absorption_input(filename, char_dict, num_wl_bins, material=material,
                                      environment=environment)

    absName = '__{}_abs_correction'.format(_getBasename(filename))

    if abs_method == "SampleOnly":
        AbsorptionCorrection(donorWS,
                             OutputWorkspace=absName + '_ass',
                             ScatterFrom='Sample',
                             ElementSize=element_size)
        return absName + '_ass', None
    elif abs_method == "SampleAndContainer":
        AbsorptionCorrection(donorWS,
                             OutputWorkspace=absName + '_ass',
                             ScatterFrom='Sample',
                             ElementSize=element_size)
        AbsorptionCorrection(donorWS,
                             OutputWorkspace=absName + '_acc',
                             ScatterFrom='Container',
                             ElementSize=element_size)
        return absName + '_ass', absName + '_acc'
    elif abs_method == "FullPaalmanPings":
        PaalmanPingsAbsorptionCorrection(donorWS,
                                         OutputWorkspace=absName,
                                         ElementSize=element_size)
        Multiply(LHSWorkspace=absName + '_acc',
                 RHSWorkspace=absName + '_assc',
                 OutputWorkspace=absName + '_ac')
        Divide(LHSWorkspace=absName + '_ac',
               RHSWorkspace=absName + '_acsc',
               OutputWorkspace=absName + '_ac')
        return absName + '_assc', absName + '_ac'
    else:
        raise RuntimeWarning("Unrecognized absorption correction method '{}'".format(abs_method))


def create_absorption_input(filename, char_dict, num_wl_bins=1000,
                            material=None, geometry=None, environment=None,
                            opt_wl_min=0, opt_wl_max=Property.EMPTY_DBL):
    """
    Create an input workspace for carpenter or other absorption corrections

    :param filename: Input file to retrieve properties from the sample log
    :param char_dict: Characterization dictionary of input, see PDDetermineCharacterizations
    :param num_wl_bins: The number of wavelength bins used for absorption correction
    :param material: Optional material to use in SetSample
    :param geometry: Optional geometry to use in SetSample
    :param environment: Optional environment to use in SetSample
    :param opt_wl_min: Optional minimum wavelength. If specified, this is used instead of from the char_dict
    :param opt_wl_max: Optional maximum wavelength. If specified, this is used instead of from the char_dict
    :return: Name of the donor workspace created
    """
    absName = '__{}_abs'.format(_getBasename(filename))

    if char_dict is None:
        raise RuntimeError("Characterization dictionary required to create donor workspace, char_dict is None")

    log = Logger('CreateAbsorptionInput')

    Load(Filename=filename, OutputWorkspace=absName, MetaDataOnly=True)

    # first attempt to get the wavelength range from the properties file
    wl_min, wl_max = char_dict['wavelength_min'].value, char_dict['wavelength_max'].value
    # override that with what was given as parameters to the algorithm
    if opt_wl_min > 0.:
        wl_min = opt_wl_min
    if opt_wl_max != Property.EMPTY_DBL:
        wl_max = opt_wl_max

    # if it isn't found by this point, guess it from the time-of-flight range
    if (wl_min == wl_max == 0.):
        tof_min = char_dict['tof_min'].value
        tof_max = char_dict['tof_max'].value
        if tof_min >= 0. and tof_max > tof_min:
            log.information('TOF range is {} to {} microseconds'.format(tof_min, tof_max))

            # determine L1
            instr = mtd[absName].getInstrument()
            L1 = instr.getSource().getDistance(instr.getSample())
            # determine L2 range
            PreprocessDetectorsToMD(InputWorkspace=absName, OutputWorkspace=absName + '_dets', GetMaskState=False)
            L2 = mtd[absName + '_dets'].column('L2')
            Lmin = np.min(L2) + L1
            Lmax = np.max(L2) + L1
            DeleteWorkspace(Workspace=absName + '_dets')

            log.information('Distance range is {} to {} meters'.format(Lmin, Lmax))

            # wavelength is h*TOF / m_n * L  values copied from Kernel/PhysicalConstants.h
            usec_to_sec = 1.e-6
            meter_to_angstrom = 1.e10
            h_m_n = meter_to_angstrom * usec_to_sec * 6.62606896e-34 / 1.674927211e-27
            wl_min = h_m_n * tof_min / Lmax
            wl_max = h_m_n * tof_max / Lmin

    # there isn't a good way to guess it so error out
    if wl_max <= wl_min:
        DeleteWorkspace(Workspace=absName)  # no longer needed
        raise RuntimeError('Invalid wavelength range min={}A max={}A'.format(wl_min, wl_max))
    log.information('Using wavelength range min={}A max={}A'.format(wl_min, wl_max))

    absorptionWS = WorkspaceFactory.create(mtd[absName], NVectors=mtd[absName].getNumberHistograms(),
                                           XLength=num_wl_bins + 1, YLength=num_wl_bins)
    xaxis = np.arange(0., float(num_wl_bins + 1)) * (wl_max - wl_min) / (num_wl_bins) + wl_min
    for i in range(absorptionWS.getNumberHistograms()):
        absorptionWS.setX(i, xaxis)
    absorptionWS.getAxis(0).setUnit('Wavelength')

    # this effectively deletes the metadata only workspace
    AnalysisDataService.addOrReplace(absName, absorptionWS)

    # Set ChemicalFormula, and either SampleMassDensity or Mass, if SampleMassDensity not set
    if material is not None:
        if (not material['ChemicalFormula']) and ("SampleFormula" in absorptionWS.run()):
            material['ChemicalFormula'] = absorptionWS.run()['SampleFormula'].lastValue().strip()
        if ("SampleMassDensity" not in material or not material['SampleMassDensity']) and (
                "SampleDensity" in absorptionWS.run()):
            if (absorptionWS.run()['SampleDensity'].lastValue() != 1.0) and (
                    absorptionWS.run()['SampleDensity'].lastValue() != 0.0):
                material['SampleMassDensity'] = absorptionWS.run()['SampleDensity'].lastValue()
            else:
                material['Mass'] = absorptionWS.run()['SampleMass'].lastValue()

    # Set height for computing density if height not set
    if geometry is None:
        geometry = {}

    if geometry is not None:
        if "Height" not in geometry or not geometry['Height']:
            # Check units - SetSample expects cm
            if absorptionWS.run()['BL11A:CS:ITEMS:HeightInContainerUnits'].lastValue() == "mm":
                conversion = 0.1
            elif absorptionWS.run()['BL11A:CS:ITEMS:HeightInContainerUnits'].lastValue() == "cm":
                conversion = 1.0
            else:
                raise ValueError("HeightInContainerUnits expects cm or mm; specified units not recognized: ",
                                 absorptionWS.run()['BL11A:CS:ITEMS:HeightInContainerUnits'].lastValue())

            geometry['Height'] = absorptionWS.run()['BL11A:CS:ITEMS:HeightInContainer'].lastValue() * conversion

    # Set container if not set
    if environment is not None:
        if environment['Container'] == "":
            environment['Container'] = absorptionWS.run()['SampleContainer'].lastValue().replace(" ", "")

    # Make sure one is set before calling SetSample
    if material or geometry or environment is not None:
        setup_sample(absName, material, geometry, environment)

    return absName


def setup_sample(donor_ws, material, geometry, environment):
    """
    Calls SetSample with the associated sample and container material and geometry for use
    in creating an input workspace for an Absorption Correction algorithm
    :param donor_ws:
    :param material:
    :param geometry:
    :param environment:
    """

    # Set the material, geometry, and container info
    SetSample(InputWorkspace=donor_ws,
              Material=material,
              Geometry=geometry,
              Environment=environment)
