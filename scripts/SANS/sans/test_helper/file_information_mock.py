from sans.common.file_information import SANSFileInformation
from mantid.kernel import DateAndTime
from sans.common.enums import (SANSFacility, SANSInstrument, FileType, SampleShape)


class SANSFileInformationMock(SANSFileInformation):
    def __init__(self, instrument=SANSInstrument.LOQ, facility=SANSFacility.ISIS, run_number=00000, file_name='file_name',
                 height=8.0, width=8.0, thickness=1.0, shape=SampleShape.Cuboid):
        super(SANSFileInformationMock, self).__init__(file_name)
        self._instrument = instrument
        self._facility = facility
        self._run_number = run_number
        self._height = height
        self._width = width
        self._thickness = thickness
        self._shape = shape

    def get_file_name(self):
        return 'file_name'

    def get_instrument(self):
        return self._instrument

    def get_facility(self):
        return self._facility

    def get_date(self):
        return DateAndTime('2012-10-22T22:41:27')

    def get_number_of_periods(self):
        return 1

    def get_run_number(self):
        return self._run_number

    def get_type(self):
        return FileType.ISISNexus

    def is_event_mode(self):
        return True

    def is_added_data(self):
        return False

    def get_height(self):
        return self._height

    def get_width(self):
        return self._width

    def get_thickness(self):
        return self._thickness

    def get_shape(self):
        return self._shape