# pylint: disable=no-init,invalid-name,too-many-locals
import mantid.simpleapi as api
from mantid.api import PythonAlgorithm, AlgorithmFactory, MatrixWorkspaceProperty
from mantid.kernel import Direction, FloatBoundedValidator
import numpy as np


class DNSFlippingRatioCorr(PythonAlgorithm):
    """
    Peforms flipping ratio correction of the given dataset
    using the NiCr data
    This algorithm is written for the DNS @ MLZ,
    but can be adjusted for other instruments if needed.
    """
    properties_to_compare = ['deterota', 'wavelength', 'slit_i_left_blade_position',
                             'slit_i_right_blade_position', 'slit_i_lower_blade_position',
                             'slit_i_upper_blade_position']
    input_workspaces = {}
    sf_outws_name = None
    nsf_outws_name = None
    dfr = 0.05

    def category(self):
        """
        Returns category
        """
        return 'PythonAlgorithms\\MLZ\\DNS\\CorrectionFunctions'

    def name(self):
        """
        Returns name
        """
        return "DNSFlippingRatioCorr"

    def summary(self):
        return "Peforms flipping ratio correction on a given dataset using the NiCr data."

    def PyInit(self):
        self.declareProperty(MatrixWorkspaceProperty("SFDataWorkspace", "", direction=Direction.Input),
                             doc="A workspace with spin-flip experimental data from sample.")
        self.declareProperty(MatrixWorkspaceProperty("NSFDataWorkspace", "", direction=Direction.Input),
                             doc="A workspace with non spin-flip experimental data from sample.")
        self.declareProperty(MatrixWorkspaceProperty("SFNiCrWorkspace", "", direction=Direction.Input),
                             doc="A workspace with spin-flip NiCr data.")
        self.declareProperty(MatrixWorkspaceProperty("NSFNiCrWorkspace", "", direction=Direction.Input),
                             doc="A workspace with non spin-flip NiCr data.")
        self.declareProperty(MatrixWorkspaceProperty("SFBkgrWorkspace", "", direction=Direction.Input),
                             doc="A workspace with spin-flip background for NiCr.")
        self.declareProperty(MatrixWorkspaceProperty("NSFBkgrWorkspace", "", direction=Direction.Input),
                             doc="A workspace with non spin-flip background for NiCr.")
        self.declareProperty(MatrixWorkspaceProperty("SFOutputWorkspace", "", direction=Direction.Output),
                             doc="A workspace to save the corrected spin-flip data.")
        self.declareProperty(MatrixWorkspaceProperty("NSFOutputWorkspace", "", direction=Direction.Output),
                             doc="A workspace to save the corrected non spin-flip data.")
        self.declareProperty("DoubleSpinFlipScatteringProbability", 0.05, FloatBoundedValidator(lower=0, upper=1.0),
                             doc="Probability of the double spin-flip scattering. Number between 0 and 1.")

        return

    def _dimensions_valid(self):
        """
        Checks validity of the workspace dimensions:
        all given workspaces must have the same number of dimensions
        and the same number of histograms
        and the same number of bins
        """
        ndims = []
        nhists = []
        nbins = []
        for wsname in self.input_workspaces.values():
            ws = api.AnalysisDataService.retrieve(wsname)
            ndims.append(ws.getNumDims())
            nhists.append(ws.getNumberHistograms())
            nbins.append(ws.blocksize())

        nd = ndims[0]
        nh = nhists[0]
        nb = nbins[0]
        if ndims.count(nd) == len(ndims) and nhists.count(nh) == len(nhists) and nbins.count(nb) == len(nbins):
            return True
        else:
            message = "Input workspaces have different dimensions. Cannot correct flipping ratio."
            self.log().error(message)
            return False

    def _ws_exist(self):
        """
        Checks whether the workspaces and their normalization workspaces exist
        """
        # normws for data workspaces are not required
        dataws_names = [self.input_workspaces['SF_Data'], self.input_workspaces['NSF_Data']]
        for wsname in self.input_workspaces.values():
            if not api.AnalysisDataService.doesExist(wsname):
                message = "Workspace " + wsname + " does not exist! Cannot correct flipping ratio."
                self.log().error(message)
                return False
            if not api.AnalysisDataService.doesExist(wsname + '_NORM'):
                if wsname not in dataws_names:
                    message = "Workspace " + wsname + "_NORM does not exist! Cannot correct flipping ratio."
                    self.log().error(message)
                    return False

        return True

    def _same_polarisation(self):
        """
        Checks whether all workspaces have the same polarisation.
        Raises error if not.
        """
        pols = []
        for wsname in self.input_workspaces.values():
            ws = api.AnalysisDataService.retrieve(wsname)
            run = ws.getRun()
            if run.hasProperty('polarisation'):
                pols.append(run.getProperty('polarisation').value)
            else:
                message = \
                    " Workspace " + ws.getName() + " does not have property polarisation! Cannot correct flipping ratio."
                self.log().error(message)
                return False
        p = pols[0]
        if pols.count(p) == len(pols):
            self.log().information("The polarisation is " + p)
            return True
        else:
            message = "Cannot apply correction to workspaces with different polarisation!"
            self.log().error(message)
            return False

    def _cleanup(self, wslist):
        """
        deletes workspaces from list
        """
        for w in wslist:
            api.DeleteWorkspace(w)
        return

    def _flipper_valid(self):
        """
        checks whether SF workspaces have flipper on
        and NSF workspaces have flipper OFF
        """
        # sort workspaces for sf and nsf
        nsf = []
        for s in self.input_workspaces.keys():
            if 'NSF' in s:
                nsf.append(s)
        for key in self.input_workspaces.keys():
            ws = api.AnalysisDataService.retrieve(self.input_workspaces[key])
            run = ws.getRun()
            if not run.hasProperty('flipper'):
                message = "Workspace " + ws.getName() + " does not have flipper sample log!"
                self.log().error(message)
                return False
            flipper = run.getProperty('flipper').value
            if key in nsf:
                needed = 'OFF'
            else:
                needed = 'ON'
            if flipper != needed:
                message = "Workspace " + ws.getName() + " must have flipper " + needed + ", but it is " + flipper
                self.log().error(message)
                return False

        return True

    def _can_correct(self):
        """
        checks whether FR correction is possible with the given list of workspaces
        """
        # list of workspaces must not be empty
        if not self.input_workspaces:
            message = "No workspace names has been specified! Nothing for FR correction."
            self.log().error(message)
            return False
        # workspaces and normalization workspaces must exist
        if not self._ws_exist():
            return False
        # they must have the same dimensions
        if not self._dimensions_valid():
            return False
        # and the same polarisation
        if not self._same_polarisation():
            return False
        # check validity of flipper status
        if not self._flipper_valid():
            return False
        # algorithm must warn if some properties_to_compare are different
        ws1 = api.AnalysisDataService.retrieve(self.input_workspaces.values()[0])
        run1 = ws1.getRun()
        for wsname in self.input_workspaces.values()[1:]:
            ws = api.AnalysisDataService.retrieve(wsname)
            run = ws.getRun()
            self._check_properties(run1, run)
        return True

    def _fr_correction(self):
        """
        applies flipping ratio correction
        creates the corrected workspaces
        """
        wslist = []
        # 1. normalize NiCr and Background
        sf_nicr_normws = api.AnalysisDataService.retrieve(self.input_workspaces['SF_NiCr'] + '_NORM')
        sf_nicr = api.AnalysisDataService.retrieve(self.input_workspaces['SF_NiCr'])
        _sf_nicr_norm_ = api.Divide(sf_nicr, sf_nicr_normws)
        wslist.append(_sf_nicr_norm_.getName())

        nsf_nicr_normws = api.AnalysisDataService.retrieve(self.input_workspaces['NSF_NiCr'] + '_NORM')
        nsf_nicr = api.AnalysisDataService.retrieve(self.input_workspaces['NSF_NiCr'])
        _nsf_nicr_norm_ = api.Divide(nsf_nicr, nsf_nicr_normws)
        wslist.append(_nsf_nicr_norm_.getName())

        sf_bkgr_normws = api.AnalysisDataService.retrieve(self.input_workspaces['SF_Background'] + '_NORM')
        sf_bkgr = api.AnalysisDataService.retrieve(self.input_workspaces['SF_Background'])
        _sf_bkgr_norm_ = api.Divide(sf_bkgr, sf_bkgr_normws)
        wslist.append(_sf_bkgr_norm_.getName())

        nsf_bkgr_normws = api.AnalysisDataService.retrieve(self.input_workspaces['NSF_Background'] + '_NORM')
        nsf_bkgr = api.AnalysisDataService.retrieve(self.input_workspaces['NSF_Background'])
        _nsf_bkgr_norm_ = api.Divide(nsf_bkgr, nsf_bkgr_normws)
        wslist.append(_nsf_bkgr_norm_.getName())

        # 2. subtract background from NiCr
        _sf_nicr_bg_ = _sf_nicr_norm_ - _sf_bkgr_norm_
        wslist.append(_sf_nicr_bg_.getName())
        _nsf_nicr_bg_ = _nsf_nicr_norm_ - _nsf_bkgr_norm_
        wslist.append(_nsf_nicr_bg_.getName())
        # check negative values, throw exception
        sf_arr = np.array(_sf_nicr_bg_.extractY()).flatten()
        nsf_arr = np.array(_nsf_nicr_bg_.extractY()).flatten()
        sf_neg_values = np.where(sf_arr < 0)[0]
        nsf_neg_values = np.where(nsf_arr < 0)[0]
        if len(sf_neg_values) or len(nsf_neg_values):
            self._cleanup(wslist)
            message = "Background is higher than NiCr signal!"
            self.log().error(message)
            raise RuntimeError(message)

        # 3. calculate correction coefficients
        _coef_ws_ = api.Divide(LHSWorkspace=_nsf_nicr_bg_, RHSWorkspace=_sf_nicr_bg_, WarnOnZeroDivide=True)
        wslist.append(_coef_ws_.getName())
        # 4. apply correction raw data (not normalized!)
        sf_data_ws = api.AnalysisDataService.retrieve(self.input_workspaces['SF_Data'])
        nsf_data_ws = api.AnalysisDataService.retrieve(self.input_workspaces['NSF_Data'])
        # NSF_corr[i] = NSF[i] - SF[i]/c[i]
        _tmp_ws_ = api.Divide(LHSWorkspace=sf_data_ws, RHSWorkspace=_coef_ws_, WarnOnZeroDivide=True,)
        api.Minus(LHSWorkspace=nsf_data_ws, RHSWorkspace=_tmp_ws_, OutputWorkspace=self.nsf_outws_name)
        nsf_outws = api.AnalysisDataService.retrieve(self.nsf_outws_name)
        api.DeleteWorkspace(_tmp_ws_)
        # SF_corr[i] = SF[i] - NSF[i]/c[i]
        _tmp_ws_ = api.Divide(LHSWorkspace=nsf_data_ws, RHSWorkspace=_coef_ws_, WarnOnZeroDivide=True)
        api.Minus(LHSWorkspace=sf_data_ws, RHSWorkspace=_tmp_ws_, OutputWorkspace=self.sf_outws_name)
        sf_outws = api.AnalysisDataService.retrieve(self.sf_outws_name)
        api.DeleteWorkspace(_tmp_ws_)

        # 5. Apply correction for a double spin-flip scattering
        if self.dfr > 1e-7:
            _tmp_ws_ = sf_outws * self.dfr
            wslist.append(_tmp_ws_.getName())
            # NSF_corr[i] = NSF_prev_corr[i] - SF_prev_corr*dfr, SF_corr = SF_prev_corr
            api.Minus(LHSWorkspace=nsf_outws, RHSWorkspace=_tmp_ws_, OutputWorkspace=self.nsf_outws_name)

        # cleanup
        self._cleanup(wslist)
        return

    def _check_properties(self, lhs_run, rhs_run):
        """
        checks whether properties match
        """
        lhs_title = ""
        rhs_title = ""
        if lhs_run.hasProperty('run_title'):
            lhs_title = lhs_run.getProperty('run_title').value
        if rhs_run.hasProperty('run_title'):
            rhs_title = rhs_run.getProperty('run_title').value

        for property_name in self.properties_to_compare:
            if lhs_run.hasProperty(property_name) and rhs_run.hasProperty(property_name):
                lhs_property = lhs_run.getProperty(property_name)
                rhs_property = rhs_run.getProperty(property_name)
                if lhs_property.type == rhs_property.type:
                    if lhs_property.type == 'string':
                        if lhs_property.value != rhs_property.value:
                            message = "Property " + property_name + " does not match! " + \
                                lhs_title + ": " + lhs_property.value + ", but " + \
                                rhs_title + ": " + rhs_property.value
                            self.log().warning(message)
                    if lhs_property.type == 'number':
                        if abs(lhs_property.value - rhs_property.value) > 5e-3:
                            message = "Property " + property_name + " does not match! " + \
                                lhs_title + ": " + str(lhs_property.value) + ", but " + \
                                rhs_title + ": " + str(rhs_property.value)
                            self.log().warning(message)
                else:
                    message = "Property " + property_name + " does not match! " + \
                        lhs_title + ": " + str(lhs_property.value) + ", but " + \
                        rhs_title + ": " + str(rhs_property.value)
                    self.log().warning(message)
            else:
                message = "Property " + property_name + " is not present in " +\
                    lhs_title + " or " + rhs_title + " - skipping comparison."
                self.log().warning(message)
        return True

    def PyExec(self):
        # Input
        self.input_workspaces['SF_Data'] = self.getPropertyValue("SFDataWorkspace")
        self.input_workspaces['NSF_Data'] = self.getPropertyValue("NSFDataWorkspace")
        self.input_workspaces['SF_NiCr'] = self.getPropertyValue("SFNiCrWorkspace")
        self.input_workspaces['NSF_NiCr'] = self.getPropertyValue("NSFNiCrWorkspace")
        self.input_workspaces['SF_Background'] = self.getPropertyValue("SFBkgrWorkspace")
        self.input_workspaces['NSF_Background'] = self.getPropertyValue("NSFBkgrWorkspace")
        self.sf_outws_name = self.getPropertyValue("SFOutputWorkspace")
        self.nsf_outws_name = self.getPropertyValue("NSFOutputWorkspace")
        self.dfr = float(self.getPropertyValue("DoubleSpinFlipScatteringProbability"))

        # check if possible to apply correction
        if not self._can_correct():
            raise RuntimeError("Error: impossible to apply flipping ratio correction to given workspaces!")

        # apply flipping ratio correction, retrieve the result
        self._fr_correction()
        nsf_outws = api.AnalysisDataService.retrieve(self.nsf_outws_name)
        sf_outws = api.AnalysisDataService.retrieve(self.sf_outws_name)

        # copy sample logs from data workspace to the output workspace
        api.CopyLogs(InputWorkspace=self.input_workspaces['SF_Data'], OutputWorkspace=self.sf_outws_name,
                     MergeStrategy='MergeReplaceExisting')
        api.CopyLogs(InputWorkspace=self.input_workspaces['NSF_Data'], OutputWorkspace=self.nsf_outws_name,
                     MergeStrategy='MergeReplaceExisting')
        self.setProperty("SFOutputWorkspace", sf_outws)
        self.setProperty("NSFOutputWorkspace", nsf_outws)

        # clone the normalization workspaces
        if api.AnalysisDataService.doesExist(self.input_workspaces['SF_Data'] + '_NORM'):
            api.CloneWorkspace(InputWorkspace=self.input_workspaces['SF_Data'] + '_NORM',
                               OutputWorkspace=self.sf_outws_name + '_NORM')
        if api.AnalysisDataService.doesExist(self.input_workspaces['NSF_Data'] + '_NORM'):
            api.CloneWorkspace(InputWorkspace=self.input_workspaces['NSF_Data'] + '_NORM',
                               OutputWorkspace=self.nsf_outws_name + '_NORM')

        return

# Register algorithm with Mantid
AlgorithmFactory.subscribe(DNSFlippingRatioCorr)
