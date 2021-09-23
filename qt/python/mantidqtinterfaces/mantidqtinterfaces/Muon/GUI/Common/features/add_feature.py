# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#   NScD Oak Ridge National Laboratory, European Spallation Source,
#   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
# SPDX - License - Identifier: GPL - 3.0 +


class addFeature(object):
    """
    Base class for adding a feature into one of the muon GUI's
    This allows a simple check to be added for if to add the feature
    """

    def __init__(self, GUI, feature_dict):
        self.feature_list = self._get_features(feature_dict)
        self._add_features(GUI)

    def _get_features(self, feature_dict):
        return []

    def _add_features(self, GUI):
        return

    def add_to_tab(self, GUI):
        return

    def add_observers_to_feature(self, GUI):
        return

    def set_feature_observables(self, GUI):
        return
