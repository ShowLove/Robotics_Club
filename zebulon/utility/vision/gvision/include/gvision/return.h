//Dirty Class to allow for less parameters

#ifndef _RETURN
#define _RETURN

namespace Zebulon
{
    namespace Vision
    {
        class Return
        {
            public:
                Return();
                Return(const Return& r)
                {
                    *this = r;
                }
                ~Return();
                Return& operator=(const Return& r)
                {
                    if(this != &r)
                    {
                        mCenterI = r.mCenterI;
                        mCenterJ = r.mCenterJ;
                        mArea = r.mArea;
                        mAngle = r.mAngle;
                        mValid = r.mValid;
                        mConfidence = r.mConfidence;
                        mMinI = r.mMinI;
                        mMinJ = r.mMinJ;
                        mMaxI = r.mMaxI;
                        mMaxJ = r.mMaxJ;
                    }
                    return *this;
                }

                double mCenterI;    ///< I is like X 0 to width = left to right
                double mCenterJ;    ///< J is like Y 0 to height  top to bottom
                int mArea;          ///< Area in pixels, either pixel count or width*height of bounding box
                double mAngle;      ///< Angle in degrees [-90, 90] positive to the right, negative left.

                int mValid;         ///< Valid is an on or off (or boolean) if something is detected
                double mConfidence; ///< Confidence in identification of target, higher is better
                
                double mMinI;   ///< Bounding Box furthest left column index.
                double mMinJ;   ///< Bounding Box lowest row value (lower value higher up).
                double mMaxI;   ///< Bounding Box furthest right column index (higher value right).
                double mMaxJ;   ///< Bounding Box highest row value (higher value is towards picture bottom).
        };
    }
}
#endif
