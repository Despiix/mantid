#ifndef MANTID_DATAOBJECTS_SPECIALWORKSPACE2D_H_
#define MANTID_DATAOBJECTS_SPECIALWORKSPACE2D_H_
    

#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/SpectraDetectorMap.h"
#include "MantidDataObjects/Workspace2D.h"
#include "MantidGeometry/IInstrument.h"
#include "MantidKernel/System.h"


namespace Mantid
{
namespace DataObjects
{

  /** An SpecialWorkspace2D is a specialized Workspace2D where
   * the Y value at each pixel will be used for a special meaning.
   * Specifically, by GroupingWorkspace and OffsetsWorkspace.
   *
   * The workspace has a single pixel per detector, and this cannot
   * be changed.
   * 
   * @author Janik Zikovsky
   * @date 2011-05-09
   */
  class DLLExport SpecialWorkspace2D  : public Workspace2D
  {
  public:
    SpecialWorkspace2D();
    SpecialWorkspace2D(Mantid::Geometry::IInstrument_sptr inst);
    ~SpecialWorkspace2D();
    
    /** Gets the name of the workspace type
    @return Standard string name  */
    virtual const std::string id() const {return "SpecialWorkspace2D";}

    /// Non-const access to the spectra map is disallowed!
    virtual Mantid::API::SpectraDetectorMap& mutableSpectraMap();

    double getValue(const int detectorID) const;
    double getValue(const int detectorID, const double defaultValue) const;

    void setValue(const int detectorID, const double value);

    int getDetectorID(const int workspaceIndex) const;

  private:
    /// Private copy constructor. NO COPY ALLOWED
    SpecialWorkspace2D(const SpecialWorkspace2D&);
    /// Private copy assignment operator. NO ASSIGNMENT ALLOWED
    SpecialWorkspace2D& operator=(const SpecialWorkspace2D&);

    virtual void init(const int &NVectors, const int &XLength, const int &YLength);

  protected:
    /** Vector with all the detector IDs, in the same order as the workspace indices.
     * Therefore, detectorIDs[workspaceIndex] = that detector ID.  */
    std::vector<int> detectorIDs;

    /// Map with key = detector ID, and value = workspace index.
    std::map<int,int> detID_to_WI;
  };


  ///shared pointer to the SpecialWorkspace2D class
  typedef boost::shared_ptr<SpecialWorkspace2D> SpecialWorkspace2D_sptr;

  ///shared pointer to a const SpecialWorkspace2D
  typedef boost::shared_ptr<const SpecialWorkspace2D> SpecialWorkspace2D_const_sptr;

} // namespace Mantid
} // namespace DataObjects

#endif  /* MANTID_DATAOBJECTS_SPECIALWORKSPACE2D_H_ */
