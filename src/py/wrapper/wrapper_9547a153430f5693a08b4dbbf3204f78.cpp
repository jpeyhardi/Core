#include "_core.h"

namespace autowig
{
    typedef ::statiskit::ContinuousUnivariateConditionalDistributionEstimation::Estimator class_type;

    class Trampoline : public class_type
    {
        public:

            typedef class ::std::unique_ptr< struct ::statiskit::UnivariateConditionalDistributionEstimation::Estimator, struct ::std::default_delete< struct ::statiskit::UnivariateConditionalDistributionEstimation::Estimator > >  return_type_e0fab2db391a546591b690a17ebe80ff;
            virtual return_type_e0fab2db391a546591b690a17ebe80ff copy() const override { PYBIND11_OVERLOAD_PURE_UNIQUE_PTR(return_type_e0fab2db391a546591b690a17ebe80ff, class_type, copy, ); };
            typedef class ::std::unique_ptr< struct ::statiskit::UnivariateConditionalDistributionEstimation, struct ::std::default_delete< struct ::statiskit::UnivariateConditionalDistributionEstimation > >  return_type_bdacedd56eba5861a3003ac87a422cf6;
            typedef ::statiskit::UnivariateConditionalDistributionEstimation::data_type const & param_bdacedd56eba5861a3003ac87a422cf6_0_type;
            typedef bool const & param_bdacedd56eba5861a3003ac87a422cf6_1_type;
            virtual return_type_bdacedd56eba5861a3003ac87a422cf6 operator()(param_bdacedd56eba5861a3003ac87a422cf6_0_type param_0, param_bdacedd56eba5861a3003ac87a422cf6_1_type param_1) const override { PYBIND11_OVERLOAD_PURE_UNIQUE_PTR(return_type_bdacedd56eba5861a3003ac87a422cf6, class_type, operator(), param_0, param_1); };
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


namespace autowig {
}

void wrapper_9547a153430f5693a08b4dbbf3204f78(pybind11::module& module)
{

    pybind11::class_<struct ::statiskit::ContinuousUnivariateConditionalDistributionEstimation::Estimator, autowig::Trampoline, autowig::HolderType< struct ::statiskit::ContinuousUnivariateConditionalDistributionEstimation::Estimator >::Type, struct ::statiskit::UnivariateConditionalDistributionEstimation::Estimator > class_9547a153430f5693a08b4dbbf3204f78(module, "Estimator", "");

}