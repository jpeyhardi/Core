#include "_core.h"

namespace autowig
{
    typedef ::statiskit::PolymorphicCopy< struct ::statiskit::SingularDistributionEstimation::Estimator, struct ::statiskit::MultinomialSingularDistributionEstimation::Estimator, struct ::statiskit::SingularDistributionEstimation::Estimator > class_type;

    class Trampoline : public class_type
    {
        public:
            using ::statiskit::PolymorphicCopy< struct ::statiskit::SingularDistributionEstimation::Estimator, struct ::statiskit::MultinomialSingularDistributionEstimation::Estimator, struct ::statiskit::SingularDistributionEstimation::Estimator >::PolymorphicCopy;

            typedef class ::std::unique_ptr< struct ::statiskit::SingularDistributionEstimation::Estimator, struct ::std::default_delete< struct ::statiskit::SingularDistributionEstimation::Estimator > >  return_type_f7ce59e3c2a75d608a6dbf9d4d96253d;
            virtual return_type_f7ce59e3c2a75d608a6dbf9d4d96253d copy() const override { PYBIND11_OVERLOAD_UNIQUE_PTR(return_type_f7ce59e3c2a75d608a6dbf9d4d96253d, class_type, copy, ); };
            typedef class ::std::unique_ptr< struct ::statiskit::SingularDistributionEstimation, struct ::std::default_delete< struct ::statiskit::SingularDistributionEstimation > >  return_type_ef6596d10b575b13b141d8bcf05ac09a;
            typedef ::statiskit::SingularDistributionEstimation::data_type const & param_ef6596d10b575b13b141d8bcf05ac09a_0_type;
            typedef bool const & param_ef6596d10b575b13b141d8bcf05ac09a_1_type;
            virtual return_type_ef6596d10b575b13b141d8bcf05ac09a operator()(param_ef6596d10b575b13b141d8bcf05ac09a_0_type param_0, param_ef6596d10b575b13b141d8bcf05ac09a_1_type param_1) const override { PYBIND11_OVERLOAD_PURE_UNIQUE_PTR(return_type_ef6596d10b575b13b141d8bcf05ac09a, class_type, operator(), param_0, param_1); };
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

void wrapper_43d603893a165ed2bf34ad286a50f22e(pybind11::module& module)
{

    pybind11::class_<struct ::statiskit::PolymorphicCopy< struct ::statiskit::SingularDistributionEstimation::Estimator, struct ::statiskit::MultinomialSingularDistributionEstimation::Estimator, struct ::statiskit::SingularDistributionEstimation::Estimator >, autowig::Trampoline, autowig::HolderType< struct ::statiskit::PolymorphicCopy< struct ::statiskit::SingularDistributionEstimation::Estimator, struct ::statiskit::MultinomialSingularDistributionEstimation::Estimator, struct ::statiskit::SingularDistributionEstimation::Estimator > >::Type, struct ::statiskit::SingularDistributionEstimation::Estimator > class_43d603893a165ed2bf34ad286a50f22e(module, "_PolymorphicCopy_43d603893a165ed2bf34ad286a50f22e", "");
    class_43d603893a165ed2bf34ad286a50f22e.def(pybind11::init<  >());

}