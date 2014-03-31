/*WIKI*
Inserts/updates a time-series log entry on a chosen workspace. The given timestamp & value are appended to the
named log entry. If the named entry does not exist then a new log is created. A time stamp must be given in
ISO8601 format, e.g. 2010-09-14T04:20:12."
*WIKI*/
/*WIKI_USAGE*
'''Python'''
 import datetime as dt

 # Add an entry for the current time
 log_name = "temperature"
 log_value = 21.5
 AddTimeSeriesLog(inOutWS, Name=log_name, Time=dt.datetime.utcnow().isoformat(), Value=log_value)

*WIKI_USAGE*/

#include "MantidAlgorithms/AddTimeSeriesLog.h"
#include "MantidKernel/MandatoryValidator.h"

namespace Mantid
{
  namespace Algorithms
  {

    // Register the algorithm into the AlgorithmFactory
    DECLARE_ALGORITHM(AddTimeSeriesLog)

    //----------------------------------------------------------------------------------------------
    /// Algorithm's name for identification. @see Algorithm::name
    const std::string AddTimeSeriesLog::name() const { return "AddTimeSeriesLog"; }

    /// Algorithm's version for identification. @see Algorithm::version
    int AddTimeSeriesLog::version() const { return 1; }

    /// Algorithm's category for identification. @see Algorithm::category
    const std::string AddTimeSeriesLog::category() const { return "DataHandling\\Logs"; }

    //----------------------------------------------------------------------------------------------
    /// Sets documentation strings for this algorithm
    void AddTimeSeriesLog::initDocs()
    {
      this->setWikiSummary("");
      this->setOptionalMessage("");
    }

    //----------------------------------------------------------------------------------------------
    /**
     * Initialize the algorithm's properties.
     */
    void AddTimeSeriesLog::init()
    {
      using namespace API;
      using namespace Kernel;
      declareProperty(new WorkspaceProperty<MatrixWorkspace>("Workspace", "", Direction::InOut),
                      "In/out workspace that will store the new log information");

      auto nonEmptyString = boost::make_shared<MandatoryValidator<std::string>>();
      declareProperty("Name", "", nonEmptyString,
                      "A string name for either a new time series log to be created "
                      "or an existing name to update", Direction::Input);
      declareProperty("Time", "", nonEmptyString,
                      "An ISO formatted date/time string specifying the timestamp for "
                      "the given log value, e.g 2010-09-14T04:20:12",
                      Direction::Input);
      auto nonEmtpyDbl = boost::make_shared<MandatoryValidator<double>>();
      declareProperty("Value", EMPTY_DBL(), nonEmtpyDbl, "The value for the log at the given time",
                      Direction::Input);
    }

    //----------------------------------------------------------------------------------------------
    /**
     * Execute the algorithm.
     */
    void AddTimeSeriesLog::exec()
    {
    }



  } // namespace Algorithms
} // namespace Mantid
