from mantid.kernel import Direction, StringArrayProperty, StringArrayLengthValidator
from mantid.api import PythonAlgorithm, AlgorithmFactory, WorkspaceProperty, WorkspaceGroup
import mantid.simpleapi as api
import numpy as np
from dateutil.parser import parse
import mlzutils


class TOFTOFMergeRuns(PythonAlgorithm):
    """ Clean the Sample Logs of workspace after merging for TOFTOF instrument
    """

    mandatory_properties = ['channel_width', 'chopper_ratio', 'chopper_speed', 'Ei', 'wavelength', 'full_channels', 'EPP']
    optional_properties = ['temperature', 'run_title']
    properties_to_merge = ['temperature', 'monitor_counts', 'duration', 'run_number', 'run_start', 'run_end']
    must_have_properties = ['monitor_counts', 'duration', 'run_number', 'run_start', 'run_end']

    def __init__(self):
        """
        Init
        """
        PythonAlgorithm.__init__(self)

    def category(self):
        """ Return category
        """
        return "PythonAlgorithms\\MLZ\\TOFTOF;Utility"

    def name(self):
        """ Return summary
        """
        return "TOFTOFMergeRuns"

    def summary(self):
        return "Merge runs and the sample logs."

    def PyInit(self):
        """ Declare properties
        """
        validator = StringArrayLengthValidator()
        validator.setLengthMin(1)
        self.declareProperty(StringArrayProperty(name="InputWorkspaces", direction=Direction.Input, validator=validator),
                             doc="Comma separated list of workspaces or groups of workspaces.")
        self.declareProperty(WorkspaceProperty("OutputWorkspace", "", direction=Direction.Output),
                             doc="Name of the workspace that will contain the merged workspaces.")
        return

    def _validate_input(self):
        """
        Checks for the valid input:
            all given workspaces and/or groups must exist
            gets names of the grouped workspaces
        """
        workspaces = self.getProperty("InputWorkspaces").value
        mlzutils.ws_exist(workspaces, self.log())
        input_workspaces = []
        if len(workspaces) < 1:
            message = "List of workspaces is empty. Nothing to merge."
            self.log().error(message)
            raise RuntimeError(message)
        for wsname in workspaces:
            wks = api.AnalysisDataService.retrieve(wsname)
            if isinstance(wks, WorkspaceGroup):
                input_workspaces.extend(wks.getNames())
            else:
                input_workspaces.append(wsname)
        return input_workspaces

    def _can_merge(self, wsnames):
        """
        Checks whether given workspaces can be merged
        """
        # mandatory properties must be identical
        api.CompareSampleLogs(wsnames, self.mandatory_properties, 0.01, 'error')

        # timing (x-axis binning) must match
        # is it possible to use WorkspaceHelpers::matchingBins from python?
        self.timingsMatch(wsnames)

        # Check sample logs for must have properties
        for wsname in wsnames:
            wks = api.AnalysisDataService.retrieve(wsname)
            run = wks.getRun()
            for prop in self.must_have_properties:
                if not run.hasProperty(prop):
                    message = "Error: Workspace " + wsname + " does not have property " + prop +\
                        ". Cannot merge."
                    self.log().error(message)
                    raise RuntimeError(message)

        # warnig if optional properties are not identical must be given
        api.CompareSampleLogs(wsnames, self.optional_properties, 0.01, 'warning')
        return True

    def PyExec(self):
        """ Main execution body
        """
        # get list of input workspaces
        input_workspace_list = self._validate_input()
        workspaceCount = len(input_workspace_list)
        self.log().information("Workspaces to merge " + str(workspaceCount))
        wsOutput = self.getPropertyValue("OutputWorkspace")

        if workspaceCount < 2:
            api.CloneWorkspace(InputWorkspace=self.wsNames[0], OutputWorkspace=wsOutput)
            self.log().warning("Cannot merge one workspace. Clone is produced.")
            return

        # check whether given workspaces can be merged
        self._can_merge(input_workspace_list)

        # delete output workspace if it exists
        if api.mtd.doesExist(wsOutput):
            api.DeleteWorkspace(Workspace=wsOutput)

        #  Merge runs
        api.MergeRuns(InputWorkspaces=input_workspace_list, OutputWorkspace=wsOutput)

        # Merge logs
        # MergeRuns by default copies all logs from the first workspace
        pdict = {}
        for prop in self.properties_to_merge:
            pdict[prop] = []

        for wsname in input_workspace_list:
            wks = api.AnalysisDataService.retrieve(wsname)
            run = wks.getRun()
            for prop in self.properties_to_merge:
                if run.hasProperty(prop):
                    pdict[prop].append(run.getProperty(prop).value)

        # take average for temperatures
        nentries = len(pdict['temperature'])
        if nentries > 0:
            temps = [float(temp) for temp in pdict['temperature']]
            tmean = sum(temps)/nentries
            api.AddSampleLog(Workspace=wsOutput, LogName='temperature', LogText=str(tmean),
                             LogType='Number', LogUnit='K')
        # sum monitor counts
        mcounts = [int(mco) for mco in pdict['monitor_counts']]
        # check for zero monitor counts
        zeros = np.where(np.array(mcounts) == 0)[0]
        if len(zeros) > 0:
            for index in zeros:
                self.log().warning("Workspace " + self.wsNames[index] + " has zero monitor counts.")
        # create sample log
        api.AddSampleLog(Workspace=wsOutput, LogName='monitor_counts', LogText=str(sum(mcounts)),
                         LogType='Number')
        # sum durations
        durations = [int(dur) for dur in pdict['duration']]
        api.AddSampleLog(Workspace=wsOutput, LogName='duration', LogText=str(sum(durations)),
                         LogType='Number', LogUnit='s')
        # get minimal run_start
        fmt = "%Y-%m-%dT%H:%M:%S%z"
        run_start = [parse(entry) for entry in pdict['run_start']]
        api.AddSampleLog(Workspace=wsOutput, LogName='run_start',
                         LogText=min(run_start).strftime(fmt), LogType='String')
        # get maximal run_end
        run_end = [parse(entry) for entry in pdict['run_end']]
        api.AddSampleLog(Workspace=wsOutput, LogName='run_end',
                         LogText=max(run_end).strftime(fmt), LogType='String')
        # list of run_numbers
        api.AddSampleLog(Workspace=wsOutput, LogName='run_number',
                         LogText=str(pdict['run_number']), LogType='String')

        self.setProperty("OutputWorkspace", wsOutput)

    def timingsMatch(self, wsNames):
        """
        :param wsNames:
        :return:
        """
        for i in range(len(wsNames)):
            leftWorkspace = wsNames[i]
            rightWorkspace = wsNames[i+1]
            leftXData = api.mtd[leftWorkspace].dataX(0)
            rightXData = api.mtd[rightWorkspace].dataX(0)
            leftDeltaX = leftXData[0] - leftXData[1]
            rightDeltaX = rightXData[0] - rightXData[1]
            if abs(leftDeltaX - rightDeltaX) >= 1e-4 or abs(rightXData[0] - leftXData[0]) >= 1e-4:
                raise RuntimeError("Timings don't match")
            else:
                return True

# Register algorithm with Mantid.
AlgorithmFactory.subscribe(TOFTOFMergeRuns)
