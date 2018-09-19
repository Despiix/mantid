from __future__ import (absolute_import, division, print_function)
from six import iteritems

from qtpy import QtGui, QtCore,QtWidgets

from Muon.GUI.Common import table_utils


class SelectSubplot(QtWidgets.QDialog):
    subplotSelectorSignal = QtCore.Signal(object)
    closeEventSignal = QtCore.Signal(object)
    def __init__(self,subplots,parent=None):
        super(SelectSubplot, self).__init__()
        self.grid = QtWidgets.QGridLayout()

        self.combo = QtWidgets.QComboBox()
        self.combo.addItems(subplots)
        self.grid.addWidget(self.combo)

        btn = QtWidgets.QPushButton("ok")
        self.grid.addWidget(btn)
        self.setLayout(self.grid)
        self.setWindowTitle("Remove Lines Subplot Selector")
        btn.clicked.connect(self.buttonClick)
    def closeEvent(self,event):
        self.closeEventSignal.emit()

    def buttonClick(self):
        pick = self.combo.currentText()
        self.subplotSelectorSignal.emit(pick)
      