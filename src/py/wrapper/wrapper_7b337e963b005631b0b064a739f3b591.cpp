#include "_core.h"

namespace autowig
{
    typedef ::statiskit::MultivariateConditionalDistributionEstimation::Estimator class_type;

    class Trampoline : public class_type
    {
        public:

            typedef class ::std::unique_ptr< struct ::statiskit::MultivariateConditionalDistributionEstimation::Estimator, struct ::std::default_delete< struct ::statiskit::MultivariateConditionalDistributionEstimation::Estimator > >  return_type_59986b4f11705d0e8aa830dfb22c3798;
            virtual return_type_59986b4f11705d0e8aa830dfb22c3798 copy() const override { PYBIND11_OVERLOAD_PURE_UNIQUE_PTR(return_type_59986b4f11705d0e8aa830dfb22c3798, class_type, copy, ); };
            typedef class ::std::unique_ptr< struct ::statiskit::MultivariateConditionalDistributionEstimation, struct ::std::default_delete< struct ::statiskit::MultivariateConditionalDistributionEstimation > >  return_type_2459c2d8b66758a8a5e14c538955ee4e;
            typedef ::statiskit::MultivariateConditionalDistributionEstimation::data_type const & param_2459c2d8b66758a8a5e14c538955ee4e_0_type;
            typedef bool const & param_2459c2d8b66758a8a5e14c538955ee4e_1_type;
            virtual return_type_2459c2d8b66758a8a5e14c538955ee4e operator()(param_2459c2d8b66758a8a5e14c538955ee4e_0_type param_0, param_2459c2d8b66758a8a5e14c538955ee4e_1_type param_1) const override { PYBIND11_OVERLOAD_PURE_UNIQUE_PTR(return_type_2459c2d8b66758a8a5e14c538955ee4e, class_type, operator(), param_0, param_1); };
            typedef ::uintptr_t  return_type_ed723b56ee6d50838db979e102419f39;
            virtual return_type_ed723b56ee6d50838db979e102419f39 identifier() const override { PYBIND11_OVERLOAD(return_type_ed723b56ee6d50838db979e102419f39, class_type, identifier, ); };
            typedef class ::std::unordered_set< unsigned long int, struct ::std::hash< unsigned long int >, struct ::std::equal_to< unsigned long int >, class ::std::allocator< unsigned long int > >  return_type_7ba100805cd95f1cba468c9ce84eb72c;
            virtual return_type_7ba100805cd95f1cba468c9ce84eb72c children() const override { PYBIND11_OVERLOAD(return_type_7ba100805cd95f1cba468c9ce84eb72c, class_type, children, ); };
    };

    class Publicist : public class_type
    {
        public:
            using class_type::identifier;
            using class_type::children;
    };
}

class ::std::unique_ptr< struct ::statiskit::MultivariateConditionalDistributionEstimation, struct ::std::default_delete< struct ::statiskit::MultivariateConditionalDistributionEstimation > >  (::statiskit::MultivariateConditionalDistributionEstimation::Estimator::*method_pointer_2459c2d8b66758a8a5e14c538955ee4e)(::statiskit::MultivariateConditionalDistributionEstimation::data_type const &, bool const &)const= &::statiskit::MultivariateConditionalDistributionEstimation::Estimator::operator();
class ::std::unique_ptr< struct ::statiskit::MultivariateConditionalDistributionEstimation::Estimator, struct ::std::default_delete< struct ::statiskit::MultivariateConditionalDistributionEstimation::Estimator > >  (::statiskit::MultivariateConditionalDistributionEstimation::Estimator::*method_pointer_59986b4f11705d0e8aa830dfb22c3798)()const= &::statiskit::MultivariateConditionalDistributionEstimation::Estimator::copy;

namespace autowig {
}

void wrapper_7b337e963b005631b0b064a739f3b591(pybind11::module& module)
{

    pybind11::class_<struct ::statiskit::MultivariateConditionalDistributionEstimation::Estimator, autowig::Trampoline, autowig::HolderType< struct ::statiskit::MultivariateConditionalDistributionEstimation::Estimator >::Type, class ::statiskit::Estimator > class_7b337e963b005631b0b064a739f3b591(module, "Estimator", "");
    class_7b337e963b005631b0b064a739f3b591.def("__call__", method_pointer_2459c2d8b66758a8a5e14c538955ee4e, "");
    class_7b337e963b005631b0b064a739f3b591.def("copy", method_pointer_59986b4f11705d0e8aa830dfb22c3798, "");

}