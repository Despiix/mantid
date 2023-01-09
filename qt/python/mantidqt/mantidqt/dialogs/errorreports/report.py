# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#   NScD Oak Ridge National Laboratory, European Spallation Source,
#   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
# SPDX - License - Identifier: GPL - 3.0 +

from qtpy import QtCore, QtGui, QtWidgets
from qtpy.QtCore import Signal, QSettings
from qtpy.QtWidgets import QMessageBox

from mantidqt.interfacemanager import InterfaceManager
from mantidqt.utils.qt import load_ui

from .details import MoreDetailsDialog

DEFAULT_PLAIN_TEXT = """Please enter any additional information about your problems. (Max 3200 characters)

For example:
    Error messages on the screen
    A script that causes the problem
    The functions you used immediately before the problem

Thank you!"""

ErrorReportUIBase, ErrorReportUI = load_ui(__file__, "errorreport.ui")


class CrashReportPage(ErrorReportUIBase, ErrorReportUI):
    action = Signal(bool, int, str, str, str)
    quit_signal = Signal()
    free_text_edited = False
    interface_manager = InterfaceManager()
    CONTACT_INFO = "ContactInfo"
    NAME = "Name"
    EMAIL = "Email"

    def __init__(self, parent=None, show_continue_terminate=False):
        super(self.__class__, self).__init__(parent)
        self.setupUi(self)
        if hasattr(self.input_free_text, "setPlaceholderText"):
            self.input_free_text.setPlaceholderText(DEFAULT_PLAIN_TEXT)
        else:
            # assume Qt<5
            self.input_free_text.setPlainText(DEFAULT_PLAIN_TEXT)
            self.input_free_text.cursorPositionChanged.connect(self.check_placeholder_text)

        self.input_text = ""
        if not show_continue_terminate:
            self.continue_terminate_frame.hide()
            self.adjustSize()

        self.quit_signal.connect(QtWidgets.QApplication.instance().quit)

        self.icon.setPixmap(QtGui.QPixmap(":/images/crying_mantid.png"))

        self.requestTextBrowser.anchorClicked.connect(self.interface_manager.showWebPage)

        self.input_name_line_edit.textChanged.connect(self.set_button_status)
        self.input_email_line_edit.textChanged.connect(self.set_button_status)
        self.input_free_text.textChanged.connect(self.set_button_status)
        self.input_free_text.textChanged.connect(self.set_plain_text_edit_field)

        self.privacy_policy_label.linkActivated.connect(self.launch_privacy_policy)

        #  The options on what to do after closing the window (exit/continue)
        self.radioButtonContinue.setChecked(True)  # Set continue to be checked by default

        #  These are the options along the bottom
        self.fullShareButton.clicked.connect(self.fullShare)
        self.nonIDShareButton.clicked.connect(self.nonIDShare)
        self.noShareButton.clicked.connect(self.noShare)

        self.setWindowFlags(QtCore.Qt.CustomizeWindowHint | QtCore.Qt.WindowTitleHint | QtCore.Qt.WindowStaysOnTopHint)
        self.setWindowModality(QtCore.Qt.ApplicationModal)

        # Dialog window to show more details of the crash to the user.
        self.details_dialog = MoreDetailsDialog(self)

        # Set default focus to the editing box, rather then letting qt try and guess
        self.input_free_text.setFocus()

        # Prefill name and email saved in QSettings
        qSettings = QSettings()
        qSettings.beginGroup(self.CONTACT_INFO)
        self.saved_name = qSettings.value(self.NAME, "", type=str)
        self.saved_email = qSettings.value(self.EMAIL, "", type=str)
        qSettings.endGroup()
        if self.saved_name or self.saved_email:
            self.input_name_line_edit.setText(self.saved_name)
            self.input_email_line_edit.setText(self.saved_email)
            self.nonIDShareButton.setEnabled(True)
            self.rememberContactInfoCheckbox.setChecked(True)

    def quit(self):
        self.quit_signal.emit()

    def fullShare(self):
        self.action.emit(self.continue_working, 0, self.input_name, self.input_email, self.input_text)
        self.close()

    def nonIDShare(self):
        self.action.emit(self.continue_working, 1, self.input_name, self.input_email, self.input_text)
        self.close()

    def noShare(self):
        self.action.emit(self.continue_working, 2, self.input_name, self.input_email, self.input_text)
        self.close()

    def get_simple_line_edit_field(self, expected_type, line_edit):
        gui_element = getattr(self, line_edit)
        value_as_string = gui_element.text()
        return expected_type(value_as_string) if value_as_string else ""

    def set_plain_text_edit_field(self):
        self.input_text = self.get_plain_text_edit_field(text_edit="input_free_text", expected_type=str)

    def get_plain_text_edit_field(self, text_edit, expected_type):
        gui_element = getattr(self, text_edit)
        value_as_string = gui_element.toPlainText()

        return expected_type(value_as_string) if value_as_string else ""

    def check_placeholder_text(self):
        if not self.free_text_edited:
            self.free_text_edited = True
            self.input_free_text.setPlainText("")

    def launch_privacy_policy(self, link):
        self.interface_manager.showWebPage(link)

    def set_button_status(self):
        if not self.input_name and not self.input_email:
            self.nonIDShareButton.setEnabled(True)
        elif self.input_name == self.saved_name and self.input_email == self.saved_email:
            self.nonIDShareButton.setEnabled(True)
        else:
            self.nonIDShareButton.setEnabled(False)

    def display_message_box(self, title, message, details):
        msg = QMessageBox(self)
        msg.setIcon(QMessageBox.Warning)
        msg.setText(message)
        msg.setWindowTitle(title)
        msg.setDetailedText(details)
        msg.setStandardButtons(QMessageBox.Ok)
        msg.setDefaultButton(QMessageBox.Ok)
        msg.setEscapeButton(QMessageBox.Ok)
        msg.exec_()

    def display_more_details(self, user_information_text, stacktrace_text):
        self.details_dialog.set_stacktrace_text(stacktrace_text)
        self.details_dialog.set_user_text(user_information_text)
        self.details_dialog.show()

    def set_report_callback(self, callback):
        self.action.connect(callback)

    @property
    def input_name(self):
        return self.get_simple_line_edit_field(line_edit="input_name_line_edit", expected_type=str)

    @property
    def input_email(self):
        return self.get_simple_line_edit_field(line_edit="input_email_line_edit", expected_type=str)

    @property
    def continue_working(self):
        return self.radioButtonContinue.isChecked()
