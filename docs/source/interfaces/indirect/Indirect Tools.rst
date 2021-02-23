Indirect Tools
==============

.. contents:: Table of Contents
  :local:

Overview
--------

The Indirect Tools interface contains useful tools that do not fit into the
other interfaces.

.. interface:: Tools
  :width: 350

Action Buttons
~~~~~~~~~~~~~~

Settings
  Opens the :ref:`Settings <interface-indirect-settings>` GUI which allows you to
  customize the settings for the Indirect interfaces.

?
  Opens this help page.

Manage Directories
  Opens the Manage Directories dialog allowing you to change your search directories
  and default save directory and enable/disable data archive search.

Transmission
------------

The Transmission tab is used to calculate the sample transmission for a given
sample on a given instrument configuration, this can then be used to determine
the sample shape and dimensions prior to an experiment.

Currently this interface supports ISIS and SNS spectrometers.

.. interface:: Tools
  :widget: tabTransmissionCalc

Options
~~~~~~~

Instrument
  Instrument intending to be used.

Analyser
  The analyser bank used for the experiment.

Reflection
  The reflection number of the planned instrument configuration.

Chemical Formula
  The chemical formula of the sample material, in the format used by the
  :ref:`SetSampleMaterial <algm-SetSampleMaterial>` algorithm.

Mass/Number Density
  The density of the sample in :math: `g/cm^3` or :math:`atoms/\mathrm{\AA{}}^3`.

Thickness
  The thickness of the sample in :math:`cm`.

Run
  Runs the processing configured on the current tab.

LoadILL
-------

The Load ILL tab is used to read data files generated by other facilities (i.e.
not ISIS or SNS). At present the only supported instruments are the ILL Indirect
Geometry spectrometers IN10, IN16 and IN13 (the latter is still a work in
progress).

The LoadILL tab operates on either *.asc* (raw data) or *.inx* (:math:`S(Q,
\omega)` reductions) files. To begin, start by selecting a relevant file using
the file browser. The interface will attempt to choose the relevant instrument
for you based on the file name. If the selected instrument currently being
displayed is not the one you desire it can be changed from selecting it in the
selection box. You may also select the relevant analyser and reflection to use.

.. interface:: Tools
  :widget: tabLoadILL

Options
~~~~~~~

Run
  Run/reduction files to load.

Instrument
  The instrument used for the experiment (set automatically based on filename if
  possible).

Analyser
  The analyser bank used for the experiment.

Reflection
  The reflection number used for the experiment.

Use Map & Map File
  Allows the filtering of spectra using a map file.

Reject Zero
  If selected, it will remove any spectra that have no counts.

Run
  Runs the processing configured on the current tab.

Plot Result
  Plotted the parameter as either a spectrum plot or contour plot upon the completion of processing.

Save Result
  Saves the result in the default save directory upon the completion of processing.

.. categories:: Interfaces Indirect