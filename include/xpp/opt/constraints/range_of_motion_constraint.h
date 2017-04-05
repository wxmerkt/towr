/**
 @file    range_of_motion_constraint.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jun 6, 2016
 @brief   Declares various Range of Motion Constraint classes
 */

#ifndef XPP_OPT_INCLUDE_RANGE_OF_MOTION_CONSTRAINT_H_
#define XPP_OPT_INCLUDE_RANGE_OF_MOTION_CONSTRAINT_H_

#include <xpp/opt/endeffectors_motion.h>
#include <xpp/time_discretization_constraint.h>
#include <memory>

namespace xpp {
namespace opt {

class BaseMotion;

/** @brief Constrains the contact to lie in a box around the nominal stance
  *
  * These constraints are necessary to avoid choosing contact locations
  * that are outside the kinematic reach of the robot. The constraint can
  * be defined in terms of joint limits or Cartesian estimates of the
  * reachability.
  *
  * This constraint calculates the position of of the contact expressed in the
  * current CoM frame and constrains it to lie in a box around the nominal/
  * natural contact position for that leg.
  */
class RangeOfMotionBox : public TimeDiscretizationConstraint {
public:
  using ComMotionPtr  = std::shared_ptr<BaseMotion>;
  using EEMotionPtr    = std::shared_ptr<EndeffectorsMotion>;
  using MaxDevXY       = std::array<double,2>;
  using NominalStance  = EndeffectorsPos;

  /**
   * @param dt discretization interval [s] when to check this constraint.
   * @param deviation_xy allowed endeffector deviation from the default (x,y).
   * @param nom nominal endeffector position in base frame.
   */
  RangeOfMotionBox(const ComMotionPtr& com_motion,
                   const EEMotionPtr& ee_motion,
                   double dt,
                   const MaxDevXY& deviation_xy,
                   const NominalStance& nom);
  virtual ~RangeOfMotionBox();

private:
  void UpdateConstraintAtInstance (double t, int k) override;
  void UpdateBoundsAtInstance (double t, int k) override;
  void UpdateJacobianAtInstance(double t, int k) override;

  int GetRow(int node, EndeffectorID ee, int dimension) const;

  MaxDevXY max_deviation_from_nominal_;
  NominalStance nominal_stance_;
  ComMotionPtr com_motion_;
  EEMotionPtr ee_motion_;
};

} /* namespace opt */
} /* namespace xpp */

#endif /* XPP_OPT_INCLUDE_RANGE_OF_MOTION_CONSTRAINT_H_ */
