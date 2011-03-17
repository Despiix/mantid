# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui/hfir_data_summary.ui'
#
# Created: Thu Mar 17 11:26:29 2011
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName("Frame")
        Frame.resize(1026, 875)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(Frame.sizePolicy().hasHeightForWidth())
        Frame.setSizePolicy(sizePolicy)
        Frame.setFrameShape(QtGui.QFrame.StyledPanel)
        Frame.setFrameShadow(QtGui.QFrame.Raised)
        self.verticalLayout = QtGui.QVBoxLayout(Frame)
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.instr_name_label = QtGui.QLabel(Frame)
        self.instr_name_label.setMinimumSize(QtCore.QSize(100, 30))
        self.instr_name_label.setMaximumSize(QtCore.QSize(100, 30))
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.instr_name_label.setFont(font)
        self.instr_name_label.setObjectName("instr_name_label")
        self.gridLayout.addWidget(self.instr_name_label, 0, 0, 1, 1)
        self.label_9 = QtGui.QLabel(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_9.sizePolicy().hasHeightForWidth())
        self.label_9.setSizePolicy(sizePolicy)
        self.label_9.setMinimumSize(QtCore.QSize(200, 27))
        self.label_9.setMaximumSize(QtCore.QSize(200, 27))
        self.label_9.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.label_9.setWordWrap(False)
        self.label_9.setIndent(0)
        self.label_9.setObjectName("label_9")
        self.gridLayout.addWidget(self.label_9, 1, 0, 1, 1)
        self.data_file_edit = QtGui.QLineEdit(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.data_file_edit.sizePolicy().hasHeightForWidth())
        self.data_file_edit.setSizePolicy(sizePolicy)
        self.data_file_edit.setMinimumSize(QtCore.QSize(300, 27))
        self.data_file_edit.setMaximumSize(QtCore.QSize(16777215, 27))
        self.data_file_edit.setBaseSize(QtCore.QSize(0, 0))
        self.data_file_edit.setObjectName("data_file_edit")
        self.gridLayout.addWidget(self.data_file_edit, 1, 1, 1, 1)
        self.data_file_browse_button = QtGui.QPushButton(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.data_file_browse_button.sizePolicy().hasHeightForWidth())
        self.data_file_browse_button.setSizePolicy(sizePolicy)
        self.data_file_browse_button.setMinimumSize(QtCore.QSize(0, 27))
        self.data_file_browse_button.setMaximumSize(QtCore.QSize(16777215, 27))
        self.data_file_browse_button.setObjectName("data_file_browse_button")
        self.gridLayout.addWidget(self.data_file_browse_button, 1, 2, 1, 1)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 1, 3, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout)
        self.reduction_options_groupbox = QtGui.QGroupBox(Frame)
        self.reduction_options_groupbox.setMinimumSize(QtCore.QSize(0, 250))
        self.reduction_options_groupbox.setMaximumSize(QtCore.QSize(16777215, 500))
        self.reduction_options_groupbox.setObjectName("reduction_options_groupbox")
        self.formLayoutWidget_2 = QtGui.QWidget(self.reduction_options_groupbox)
        self.formLayoutWidget_2.setGeometry(QtCore.QRect(10, 20, 881, 222))
        self.formLayoutWidget_2.setObjectName("formLayoutWidget_2")
        self.gridLayout_2 = QtGui.QGridLayout(self.formLayoutWidget_2)
        self.gridLayout_2.setSizeConstraint(QtGui.QLayout.SetFixedSize)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.label_4 = QtGui.QLabel(self.formLayoutWidget_2)
        self.label_4.setObjectName("label_4")
        self.gridLayout_2.addWidget(self.label_4, 0, 0, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.normalization_none_radio = QtGui.QRadioButton(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.normalization_none_radio.sizePolicy().hasHeightForWidth())
        self.normalization_none_radio.setSizePolicy(sizePolicy)
        self.normalization_none_radio.setMinimumSize(QtCore.QSize(100, 0))
        self.normalization_none_radio.setMaximumSize(QtCore.QSize(100, 16777215))
        self.normalization_none_radio.setObjectName("normalization_none_radio")
        self.horizontalLayout.addWidget(self.normalization_none_radio)
        self.normalization_time_radio = QtGui.QRadioButton(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.normalization_time_radio.sizePolicy().hasHeightForWidth())
        self.normalization_time_radio.setSizePolicy(sizePolicy)
        self.normalization_time_radio.setMinimumSize(QtCore.QSize(100, 0))
        self.normalization_time_radio.setMaximumSize(QtCore.QSize(100, 16777215))
        self.normalization_time_radio.setObjectName("normalization_time_radio")
        self.horizontalLayout.addWidget(self.normalization_time_radio)
        self.normalization_monitor_radio = QtGui.QRadioButton(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.normalization_monitor_radio.sizePolicy().hasHeightForWidth())
        self.normalization_monitor_radio.setSizePolicy(sizePolicy)
        self.normalization_monitor_radio.setMinimumSize(QtCore.QSize(100, 0))
        self.normalization_monitor_radio.setMaximumSize(QtCore.QSize(100, 16777215))
        self.normalization_monitor_radio.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.normalization_monitor_radio.setObjectName("normalization_monitor_radio")
        self.horizontalLayout.addWidget(self.normalization_monitor_radio)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem1)
        self.gridLayout_2.addLayout(self.horizontalLayout, 0, 1, 1, 1)
        self.label_2 = QtGui.QLabel(self.formLayoutWidget_2)
        self.label_2.setObjectName("label_2")
        self.gridLayout_2.addWidget(self.label_2, 1, 0, 1, 1)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label = QtGui.QLabel(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QtCore.QSize(0, 0))
        self.label.setMaximumSize(QtCore.QSize(150, 16777215))
        self.label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label.setObjectName("label")
        self.horizontalLayout_2.addWidget(self.label)
        self.n_q_bins_edit = QtGui.QLineEdit(self.formLayoutWidget_2)
        self.n_q_bins_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.n_q_bins_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.n_q_bins_edit.setObjectName("n_q_bins_edit")
        self.horizontalLayout_2.addWidget(self.n_q_bins_edit)
        self.label_3 = QtGui.QLabel(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy)
        self.label_3.setMinimumSize(QtCore.QSize(0, 0))
        self.label_3.setMaximumSize(QtCore.QSize(150, 16777215))
        self.label_3.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout_2.addWidget(self.label_3)
        self.n_sub_pix_edit = QtGui.QLineEdit(self.formLayoutWidget_2)
        self.n_sub_pix_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.n_sub_pix_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.n_sub_pix_edit.setObjectName("n_sub_pix_edit")
        self.horizontalLayout_2.addWidget(self.n_sub_pix_edit)
        self.log_binning_radio = QtGui.QCheckBox(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.log_binning_radio.sizePolicy().hasHeightForWidth())
        self.log_binning_radio.setSizePolicy(sizePolicy)
        self.log_binning_radio.setMinimumSize(QtCore.QSize(120, 0))
        self.log_binning_radio.setMaximumSize(QtCore.QSize(120, 16777215))
        self.log_binning_radio.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.log_binning_radio.setObjectName("log_binning_radio")
        self.horizontalLayout_2.addWidget(self.log_binning_radio)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem2)
        self.gridLayout_2.addLayout(self.horizontalLayout_2, 1, 1, 1, 1)
        self.solid_angle_chk = QtGui.QCheckBox(self.formLayoutWidget_2)
        self.solid_angle_chk.setObjectName("solid_angle_chk")
        self.gridLayout_2.addWidget(self.solid_angle_chk, 2, 0, 1, 1)
        self.sensitivity_chk = QtGui.QCheckBox(self.formLayoutWidget_2)
        self.sensitivity_chk.setObjectName("sensitivity_chk")
        self.gridLayout_2.addWidget(self.sensitivity_chk, 3, 0, 1, 1)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.sensitivity_file_edit = QtGui.QLineEdit(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_file_edit.sizePolicy().hasHeightForWidth())
        self.sensitivity_file_edit.setSizePolicy(sizePolicy)
        self.sensitivity_file_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.sensitivity_file_edit.setBaseSize(QtCore.QSize(0, 0))
        self.sensitivity_file_edit.setObjectName("sensitivity_file_edit")
        self.horizontalLayout_4.addWidget(self.sensitivity_file_edit)
        self.sensitivity_browse_button = QtGui.QPushButton(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_browse_button.sizePolicy().hasHeightForWidth())
        self.sensitivity_browse_button.setSizePolicy(sizePolicy)
        self.sensitivity_browse_button.setObjectName("sensitivity_browse_button")
        self.horizontalLayout_4.addWidget(self.sensitivity_browse_button)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem3)
        self.gridLayout_2.addLayout(self.horizontalLayout_4, 5, 1, 1, 1)
        self.sensitivity_range_label = QtGui.QLabel(self.formLayoutWidget_2)
        self.sensitivity_range_label.setIndent(50)
        self.sensitivity_range_label.setObjectName("sensitivity_range_label")
        self.gridLayout_2.addWidget(self.sensitivity_range_label, 7, 0, 1, 1)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.sensitivity_min_label = QtGui.QLabel(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_min_label.sizePolicy().hasHeightForWidth())
        self.sensitivity_min_label.setSizePolicy(sizePolicy)
        self.sensitivity_min_label.setMinimumSize(QtCore.QSize(0, 0))
        self.sensitivity_min_label.setMaximumSize(QtCore.QSize(150, 16777215))
        self.sensitivity_min_label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.sensitivity_min_label.setObjectName("sensitivity_min_label")
        self.horizontalLayout_5.addWidget(self.sensitivity_min_label)
        self.min_sensitivity_edit = QtGui.QLineEdit(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.min_sensitivity_edit.sizePolicy().hasHeightForWidth())
        self.min_sensitivity_edit.setSizePolicy(sizePolicy)
        self.min_sensitivity_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.min_sensitivity_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.min_sensitivity_edit.setObjectName("min_sensitivity_edit")
        self.horizontalLayout_5.addWidget(self.min_sensitivity_edit)
        self.sensitivity_max_label = QtGui.QLabel(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_max_label.sizePolicy().hasHeightForWidth())
        self.sensitivity_max_label.setSizePolicy(sizePolicy)
        self.sensitivity_max_label.setMinimumSize(QtCore.QSize(0, 0))
        self.sensitivity_max_label.setMaximumSize(QtCore.QSize(150, 16777215))
        self.sensitivity_max_label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.sensitivity_max_label.setObjectName("sensitivity_max_label")
        self.horizontalLayout_5.addWidget(self.sensitivity_max_label)
        self.max_sensitivity_edit = QtGui.QLineEdit(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.max_sensitivity_edit.sizePolicy().hasHeightForWidth())
        self.max_sensitivity_edit.setSizePolicy(sizePolicy)
        self.max_sensitivity_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.max_sensitivity_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.max_sensitivity_edit.setObjectName("max_sensitivity_edit")
        self.horizontalLayout_5.addWidget(self.max_sensitivity_edit)
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_5.addItem(spacerItem4)
        self.gridLayout_2.addLayout(self.horizontalLayout_5, 7, 1, 1, 1)
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName("horizontalLayout_6")
        self.sensitivity_dark_file_edit = QtGui.QLineEdit(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_dark_file_edit.sizePolicy().hasHeightForWidth())
        self.sensitivity_dark_file_edit.setSizePolicy(sizePolicy)
        self.sensitivity_dark_file_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.sensitivity_dark_file_edit.setBaseSize(QtCore.QSize(0, 0))
        self.sensitivity_dark_file_edit.setObjectName("sensitivity_dark_file_edit")
        self.horizontalLayout_6.addWidget(self.sensitivity_dark_file_edit)
        self.sensitivity_dark_browse_button = QtGui.QPushButton(self.formLayoutWidget_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_dark_browse_button.sizePolicy().hasHeightForWidth())
        self.sensitivity_dark_browse_button.setSizePolicy(sizePolicy)
        self.sensitivity_dark_browse_button.setObjectName("sensitivity_dark_browse_button")
        self.horizontalLayout_6.addWidget(self.sensitivity_dark_browse_button)
        spacerItem5 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_6.addItem(spacerItem5)
        self.gridLayout_2.addLayout(self.horizontalLayout_6, 6, 1, 1, 1)
        self.sensitivity_file_label = QtGui.QLabel(self.formLayoutWidget_2)
        self.sensitivity_file_label.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.sensitivity_file_label.setWordWrap(False)
        self.sensitivity_file_label.setIndent(50)
        self.sensitivity_file_label.setObjectName("sensitivity_file_label")
        self.gridLayout_2.addWidget(self.sensitivity_file_label, 5, 0, 1, 1)
        self.sensitivity_dark_file_label = QtGui.QLabel(self.formLayoutWidget_2)
        self.sensitivity_dark_file_label.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.sensitivity_dark_file_label.setWordWrap(False)
        self.sensitivity_dark_file_label.setIndent(50)
        self.sensitivity_dark_file_label.setObjectName("sensitivity_dark_file_label")
        self.gridLayout_2.addWidget(self.sensitivity_dark_file_label, 6, 0, 1, 1)
        self.verticalLayout.addWidget(self.reduction_options_groupbox)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.geometry_options_groupbox = QtGui.QGroupBox(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.geometry_options_groupbox.sizePolicy().hasHeightForWidth())
        self.geometry_options_groupbox.setSizePolicy(sizePolicy)
        self.geometry_options_groupbox.setMinimumSize(QtCore.QSize(0, 200))
        self.geometry_options_groupbox.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.geometry_options_groupbox.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.geometry_options_groupbox.setObjectName("geometry_options_groupbox")
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.geometry_options_groupbox)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.formLayout_3 = QtGui.QFormLayout()
        self.formLayout_3.setSizeConstraint(QtGui.QLayout.SetFixedSize)
        self.formLayout_3.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout_3.setObjectName("formLayout_3")
        self.detector_offset_chk = QtGui.QCheckBox(self.geometry_options_groupbox)
        self.detector_offset_chk.setWhatsThis("")
        self.detector_offset_chk.setObjectName("detector_offset_chk")
        self.formLayout_3.setWidget(0, QtGui.QFormLayout.LabelRole, self.detector_offset_chk)
        self.detector_offset_edit = QtGui.QLineEdit(self.geometry_options_groupbox)
        self.detector_offset_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.detector_offset_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.detector_offset_edit.setObjectName("detector_offset_edit")
        self.formLayout_3.setWidget(0, QtGui.QFormLayout.FieldRole, self.detector_offset_edit)
        self.sample_dist_chk = QtGui.QCheckBox(self.geometry_options_groupbox)
        self.sample_dist_chk.setObjectName("sample_dist_chk")
        self.formLayout_3.setWidget(1, QtGui.QFormLayout.LabelRole, self.sample_dist_chk)
        self.sample_dist_edit = QtGui.QLineEdit(self.geometry_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sample_dist_edit.sizePolicy().hasHeightForWidth())
        self.sample_dist_edit.setSizePolicy(sizePolicy)
        self.sample_dist_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.sample_dist_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.sample_dist_edit.setObjectName("sample_dist_edit")
        self.formLayout_3.setWidget(1, QtGui.QFormLayout.FieldRole, self.sample_dist_edit)
        self.wavelength_chk = QtGui.QCheckBox(self.geometry_options_groupbox)
        self.wavelength_chk.setObjectName("wavelength_chk")
        self.formLayout_3.setWidget(2, QtGui.QFormLayout.LabelRole, self.wavelength_chk)
        self.wavelength_edit = QtGui.QLineEdit(self.geometry_options_groupbox)
        self.wavelength_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.wavelength_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.wavelength_edit.setObjectName("wavelength_edit")
        self.formLayout_3.setWidget(2, QtGui.QFormLayout.FieldRole, self.wavelength_edit)
        self.label_10 = QtGui.QLabel(self.geometry_options_groupbox)
        self.label_10.setIndent(23)
        self.label_10.setObjectName("label_10")
        self.formLayout_3.setWidget(3, QtGui.QFormLayout.LabelRole, self.label_10)
        self.wavelength_spread_edit = QtGui.QLineEdit(self.geometry_options_groupbox)
        self.wavelength_spread_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.wavelength_spread_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.wavelength_spread_edit.setObjectName("wavelength_spread_edit")
        self.formLayout_3.setWidget(3, QtGui.QFormLayout.FieldRole, self.wavelength_spread_edit)
        self.verticalLayout_2.addLayout(self.formLayout_3)
        spacerItem6 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_2.addItem(spacerItem6)
        self.horizontalLayout_3.addWidget(self.geometry_options_groupbox)
        self.groupBox = QtGui.QGroupBox(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBox.sizePolicy().hasHeightForWidth())
        self.groupBox.setSizePolicy(sizePolicy)
        self.groupBox.setMinimumSize(QtCore.QSize(300, 150))
        self.groupBox.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.groupBox.setObjectName("groupBox")
        self.gridLayout_4 = QtGui.QGridLayout(self.groupBox)
        self.gridLayout_4.setObjectName("gridLayout_4")
        self.placeholder_layout = QtGui.QHBoxLayout()
        self.placeholder_layout.setSizeConstraint(QtGui.QLayout.SetDefaultConstraint)
        self.placeholder_layout.setObjectName("placeholder_layout")
        self.gridLayout_4.addLayout(self.placeholder_layout, 0, 0, 1, 1)
        self.horizontalLayout_3.addWidget(self.groupBox)
        spacerItem7 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem7)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        spacerItem8 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem8)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.instr_name_label.setText(QtGui.QApplication.translate("Frame", "BIOSANS", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("Frame", "Scattering data file:", None, QtGui.QApplication.UnicodeUTF8))
        self.data_file_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter a valid file path to the SANS data file you want to reduce.", None, QtGui.QApplication.UnicodeUTF8))
        self.data_file_browse_button.setText(QtGui.QApplication.translate("Frame", "Browse...", None, QtGui.QApplication.UnicodeUTF8))
        self.reduction_options_groupbox.setTitle(QtGui.QApplication.translate("Frame", "Reduction Options", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("Frame", "Normalization", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_none_radio.setToolTip(QtGui.QApplication.translate("Frame", "Select to skip data normalization.", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_none_radio.setText(QtGui.QApplication.translate("Frame", "None", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_time_radio.setToolTip(QtGui.QApplication.translate("Frame", "Select to normalize the data to the data collection time.", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_time_radio.setText(QtGui.QApplication.translate("Frame", "Time", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_monitor_radio.setToolTip(QtGui.QApplication.translate("Frame", "Select to normalize the data to the beam monitor count.", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_monitor_radio.setText(QtGui.QApplication.translate("Frame", "Monitor", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Frame", "Radial averaging", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Frame", "Number of Q bins  ", None, QtGui.QApplication.UnicodeUTF8))
        self.n_q_bins_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the number of Q bins for the output I(Q) distribution.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Frame", "Number of sub-pixels", None, QtGui.QApplication.UnicodeUTF8))
        self.n_sub_pix_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the number of sub-pixels in each direction of a detector pixel to use for the\n"
"radial averaging. For instance, entering 3 will sub-divide each detector pixel by 3\n"
"in each direction and will create 9 sub-pixels.", None, QtGui.QApplication.UnicodeUTF8))
        self.log_binning_radio.setToolTip(QtGui.QApplication.translate("Frame", "Select to use logarithmic binning for I(Q)", None, QtGui.QApplication.UnicodeUTF8))
        self.log_binning_radio.setText(QtGui.QApplication.translate("Frame", "Log binning", None, QtGui.QApplication.UnicodeUTF8))
        self.solid_angle_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to perform a solid angle correction.", None, QtGui.QApplication.UnicodeUTF8))
        self.solid_angle_chk.setText(QtGui.QApplication.translate("Frame", "Perform solid angle correction", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to perform a detector sensitivity correction.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_chk.setText(QtGui.QApplication.translate("Frame", "Perform sensitivity correction", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_file_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter a valid file path to be used for the detector sensitivity data.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_browse_button.setText(QtGui.QApplication.translate("Frame", "Browse...", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_range_label.setText(QtGui.QApplication.translate("Frame", "Allowed sensitivity range", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_min_label.setText(QtGui.QApplication.translate("Frame", "Min", None, QtGui.QApplication.UnicodeUTF8))
        self.min_sensitivity_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the minimum allowed relative sensitivity for any given pixel.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_max_label.setText(QtGui.QApplication.translate("Frame", "Max", None, QtGui.QApplication.UnicodeUTF8))
        self.max_sensitivity_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the maximum allowed relative sensitivity for any given pixel.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_dark_file_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter a valid file path to be used for the dark current data.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_dark_browse_button.setText(QtGui.QApplication.translate("Frame", "Browse...", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_file_label.setText(QtGui.QApplication.translate("Frame", "Sensitivity data file", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_dark_file_label.setText(QtGui.QApplication.translate("Frame", "Dark current data file", None, QtGui.QApplication.UnicodeUTF8))
        self.geometry_options_groupbox.setTitle(QtGui.QApplication.translate("Frame", "Experiment Options", None, QtGui.QApplication.UnicodeUTF8))
        self.detector_offset_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to set the detector distance offset.", None, QtGui.QApplication.UnicodeUTF8))
        self.detector_offset_chk.setText(QtGui.QApplication.translate("Frame", "Detector distance offset [mm]", None, QtGui.QApplication.UnicodeUTF8))
        self.detector_offset_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the detector distance offset in mm.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_dist_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to force the sample-detector distance.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_dist_chk.setText(QtGui.QApplication.translate("Frame", "Sample-detector distance [mm]", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_dist_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the value of the sample-to-detector distance in mm.", None, QtGui.QApplication.UnicodeUTF8))
        self.wavelength_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to force the wavelength to a given value.", None, QtGui.QApplication.UnicodeUTF8))
        self.wavelength_chk.setText(QtGui.QApplication.translate("Frame", "Wavelength [Angstrom]", None, QtGui.QApplication.UnicodeUTF8))
        self.wavelength_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the value of the neutron wavelength.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setText(QtGui.QApplication.translate("Frame", "Wavelength spread [Angstrom]", None, QtGui.QApplication.UnicodeUTF8))
        self.wavelength_spread_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the value of the neutron wavelength spread.", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("Frame", "Edit Mask", None, QtGui.QApplication.UnicodeUTF8))

