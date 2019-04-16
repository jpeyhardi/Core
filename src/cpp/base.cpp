#include <string>
#include <iostream>
#include <limits>
#include <memory>

#include "base.h"

namespace statiskit
{
    namespace __impl
    {
        double reldiff(const double& prev, const double& curr)
        { return fabs(curr-prev) / fabs(prev); }

        double reldiff(const Eigen::VectorXd& prev, const Eigen::VectorXd& curr)
        {
            // double norm = 0; 
            // for(Index i = 0, max_index = prev.size(); i < max_index; ++i)
            // {
            //     if(prev[i] == 0)
            //     { norm +=  pow(curr[i], 2); }
            //     else
            //     { norm +=  pow((prev[i]-curr[i])/prev[i], 2); } 
            // }
            // return sqrt(norm); 
            double norm_diff = 0, norm=0; 
            for(Index i = 0, max_index = prev.size(); i < max_index; ++i)
            {
                norm_diff +=  pow(curr[i]-prev[i], 2);
                norm += pow(prev[i], 2);
            }
            if(norm != 0)
            { return sqrt(norm_diff)/sqrt(norm); }
            else
            { return sqrt(norm_diff); }
        }

        double reldiff(const Eigen::MatrixXd& prev, const Eigen::MatrixXd& curr)
        {
            // double norm = 0; 
            // for(Index i = 0, max_index = prev.rows(); i < max_index; ++i)
            // {
            //     for(Index j = 0, max_index = prev.cols(); j < max_index; ++j)
            //     {
            //         if(prev(i,j) == 0)
            //         { norm +=  pow(curr(i,j), 2); }
            //         else
            //         { norm +=  pow((prev(i,j)-curr(i,j))/prev(i,j), 2); }                     
            //     }
            // }
            // return sqrt(norm); 

            double norm_diff = 0, norm = 0; 
            for(Index i = 0, max_index = prev.rows(); i < max_index; ++i)
            {
                for(Index j = 0, max_index = prev.cols(); j < max_index; ++j)
                {
                    norm_diff +=  pow(curr(i,j)-prev(i,j), 2);
                    norm += pow(prev(i,j), 2);
                }  
            }
            if(norm != 0)
            { return sqrt(norm_diff)/sqrt(norm); }
            else
            { return sqrt(norm_diff); }         
        }

        boost::mt19937 _random_generator = boost::mt19937(0);

        boost::mt19937& get_random_generator()
        { return _random_generator; }

        std::unordered_map< uintptr_t, unsigned int > iterations = std::unordered_map< uintptr_t, unsigned int >();

        unsigned int get_maxits(const uintptr_t& ptr, const unsigned int& maxits)
        {
            unsigned int _maxits;
            std::unordered_map< uintptr_t, unsigned int >::iterator it = iterations.find(ptr);
            if(it == iterations.end())
            { _maxits = maxits; }
            else
            { _maxits = it->second; }
            return _maxits;
        }

        void set_maxits(const uintptr_t& ptr, const unsigned int& maxits)
        {
            std::unordered_map< uintptr_t, unsigned int >::iterator it = iterations.find(ptr);
            if(it == iterations.end())
            { iterations[ptr] = maxits; }
            else
            { it->second = maxits; }
        }

        void unset_maxits(const uintptr_t& ptr)
        {
            std::unordered_map< uintptr_t, unsigned int >::iterator it = iterations.find(ptr);
            if(it != iterations.end())
            { iterations.erase(it); }
        }
    }
    
    void set_seed()
    { __impl::_random_generator.seed(); }

    void set_seed(const Index& seed)
    { __impl::_random_generator.seed(seed); }

    not_implemented_error::not_implemented_error(const std::string& function) : std::runtime_error("'" + function + "' is not yet implemented")
    {}

    proxy_connection_error::proxy_connection_error() : std::exception()
    {}

    parameter_error::parameter_error(const std::string& parameter, const std::string& error) : std::runtime_error("'" + parameter + "' parameter: " + error)
    {}

    member_error::member_error(const std::string& member, const std::string& error) : std::runtime_error("'" + member + "' member: " + error)
    {}

    size_error::size_error(const std::string& parameter, const Index& self, const Index& other, const size_type& size) : parameter_error(parameter, "size of " + __impl::to_string(self) + " instead of " + __impl::to_string(other))
    {}

    nullptr_error::nullptr_error(const std::string& parameter) : parameter_error(parameter, "cannot be set to nullptr")
    {}

    Schedule::~Schedule()
    {}

    ExponentialSchedule::ExponentialSchedule(const double& theta)
    { _theta = theta; }

    ExponentialSchedule::ExponentialSchedule(const ExponentialSchedule& schedule)
    { _theta = schedule._theta; }

    ExponentialSchedule::~ExponentialSchedule()
    {}

    double ExponentialSchedule::operator() (const double& stage) const
    { return exp(- stage / _theta); }

    const double& ExponentialSchedule::get_theta() const
    { return _theta; }

    void ExponentialSchedule::set_theta(const double& theta)
    { _theta = theta; }
}