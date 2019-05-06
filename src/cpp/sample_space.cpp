#include "sample_space.h"
#include "data.h"

namespace statiskit
{
    UnivariateSampleSpace::~UnivariateSampleSpace()
    {}

    CategoricalSampleSpace::CategoricalSampleSpace(const std::set< std::string >& values)
    { _values = values; }

    CategoricalSampleSpace::CategoricalSampleSpace(const CategoricalSampleSpace& sample_space) 
    { 
    	_values = sample_space._values; 
    	_encoding = sample_space._encoding;
    }

    CategoricalSampleSpace::~CategoricalSampleSpace() 
    {}

    const std::set< std::string >& CategoricalSampleSpace::get_values() const
    { return _values; }
    
    encoding_type CategoricalSampleSpace::get_encoding() const
    { return _encoding; }
    
    Index CategoricalSampleSpace::get_cardinality() const
    { return _values.size(); }

    outcome_type CategoricalSampleSpace::get_outcome() const
    { return CATEGORICAL; }

    bool CategoricalSampleSpace::is_compatible(const UnivariateEvent* event) const
    {
        bool compatible = !event;
        if(!compatible)
        {
            if(event->get_outcome() == CATEGORICAL)
            {
                switch(event->get_event())
                {
                    case ELEMENTARY:
                        compatible = is_compatible_value(static_cast< const CategoricalElementaryEvent* >(event)->get_value()); //_values.find(static_cast< const CategoricalElementaryEvent* >(event)->get_value()) != _values.cend();
                        break;
                    case CENSORED:
                        {
                            const std::vector< std::string >& __values = static_cast< const CategoricalCensoredEvent* >(event)->get_values();
                            std::vector< std::string >::const_iterator it = __values.cbegin(), ite = __values.cend();
                            compatible = true;
                            while(compatible && it != ite)
                            {
                                compatible = is_compatible_value(*it);
                                ++it;
                            }
                        }
                        break;
                    default:
                        compatible = false;
                        break;
                }
            }
        }
        return compatible;
    }

    bool CategoricalSampleSpace::is_compatible_value(const std::string& value) const
    { return _values.find(value) != _values.end(); }

    NominalSampleSpace::NominalSampleSpace(const std::set< std::string >& values) : CategoricalSampleSpace(values)
    { 
        _reference = _values.cend();
        --_reference;
        _encoding = TREATMENT;
    }

    NominalSampleSpace::NominalSampleSpace(const NominalSampleSpace& sample_space) : CategoricalSampleSpace(sample_space)
    {
        _reference = _values.cbegin();
        advance(_reference, distance(sample_space._values.cbegin(), sample_space._reference));
        _encoding = sample_space._encoding;
    }

    NominalSampleSpace::~NominalSampleSpace()
    {}

    ordering_type NominalSampleSpace::get_ordering() const
    { return NONE; }

    const std::string& NominalSampleSpace::get_reference() const
    { return *_reference; }

    void NominalSampleSpace::set_reference(const std::string& reference)
    {
        std::set< std::string >::const_iterator it = _values.find(reference);
        if(it != _values.cend())
        { _reference = it; }
        else
        { throw std::runtime_error("invalid reference"); }
    }
       
    void NominalSampleSpace::randomize()
    {
        _reference = _values.cbegin();
        boost::random::uniform_int_distribution<> dist(0, get_cardinality()-1);
        boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<>  > simulator(__impl::get_random_generator(), dist);
        advance(_reference, simulator());
    }

    void NominalSampleSpace::set_encoding(const encoding_type& encoding)
    {
        if(encoding > DEVIATION)
        { throw std::runtime_error("invalid encoding"); }
        _encoding = encoding;
    }

    Eigen::RowVectorXd NominalSampleSpace::encode(const std::string& value) const
    {
        Eigen::RowVectorXd dummy;
        Index cardinality = get_cardinality();
        if(cardinality > 1)
        {
            --cardinality;
            std::set< std::string >::const_iterator it = _values.find(value);
            if(it == _values.cend())
            { dummy = std::numeric_limits< double >::quiet_NaN() * Eigen::RowVectorXd::Ones(cardinality); }
            else
            {
                Index index = distance(_values.cbegin(), it), ref_index = distance(_values.cbegin(), _reference);
                switch(_encoding)
                {
                    case TREATMENT:
                        dummy = Eigen::RowVectorXd::Zero(cardinality);
                        if(index < ref_index)
                        { dummy(index) = 1; }
                        else if(index > ref_index)
                        { 
                            --index;
                            dummy(index) = 1;
                        }
                        break;
                    case DEVIATION:
                        if(index == ref_index)
                        { dummy = -1 * Eigen::RowVectorXd::Ones(cardinality); }
                        else
                        {
                            dummy = Eigen::RowVectorXd::Zero(cardinality);
                            if(index < ref_index)
                            { dummy(index) = 1; }
                            else if(index > ref_index)
                            { 
                                --index;
                                dummy(index) = 1;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        else
        { dummy = Eigen::RowVectorXd(); }
        return dummy;
    }

    std::unique_ptr< OrdinalSampleSpace > NominalSampleSpace::as_ordinal() const
    { return std::make_unique< OrdinalSampleSpace >(std::vector< std::string >(_values.cbegin(), _values.cend())); }

    std::unique_ptr< UnivariateSampleSpace > NominalSampleSpace::copy() const
    { return std::make_unique< NominalSampleSpace >(*this); }

    OrdinalSampleSpace::OrdinalSampleSpace(const std::vector< std::string >& values) : CategoricalSampleSpace(std::set< std::string >(values.cbegin(), values.cend()))
    {
        _encoding = CUMULATIVE;
        _rank = std::vector< Index >(_values.size());
        for(Index size = 0, max_size = _values.size(); size < max_size; ++size)
        { _rank[distance(_values.begin(), _values.find(values[size]))] = size; }
    }

    OrdinalSampleSpace::OrdinalSampleSpace(const OrdinalSampleSpace& sample_space) : CategoricalSampleSpace(sample_space)
    {
        _rank = sample_space._rank;
        _encoding = sample_space._encoding;
    }

    OrdinalSampleSpace::~OrdinalSampleSpace()
    {}

    ordering_type OrdinalSampleSpace::get_ordering() const
    { return TOTAL; }

    void OrdinalSampleSpace::set_encoding(const encoding_type& encoding)
    { _encoding = encoding; }

    Eigen::RowVectorXd OrdinalSampleSpace::encode(const std::string& value) const
    {
        Eigen::RowVectorXd dummy;
        Index cardinality = get_cardinality();
        if(cardinality > 1)
        {
            --cardinality;
            std::set< std::string >::const_iterator it = _values.find(value);
            Index index, max_index;
            if(it == _values.cend())
            { dummy = std::numeric_limits< double >::quiet_NaN() * Eigen::RowVectorXd::Ones(cardinality); }
            else
            {
                switch(_encoding)
                {
                    case TREATMENT:
                        index = _rank[distance(_values.cbegin(), it)];
                        dummy = Eigen::RowVectorXd::Zero(cardinality);
                        if(index < cardinality)
                        { dummy(index) = 1; }
                        break;
                    case DEVIATION:
                        if(index == cardinality)
                        { dummy = -1 * Eigen::RowVectorXd::Ones(cardinality); }
                        else
                        {
                            dummy = Eigen::RowVectorXd::Zero(cardinality);
                            if(index < cardinality)
                            { dummy(index) = 1; }
                        }
                        break;
                    case CUMULATIVE:
                        dummy = Eigen::RowVectorXd::Zero(cardinality);
                        for(index = 0, max_index = std::min(cardinality, _rank[distance(_values.cbegin(), it)]); index < max_index; ++index)
                        { dummy(index) = 1; } 
                        break;
                }
            }
        }
        else
        { dummy = Eigen::RowVectorXd(); }
        return dummy;
    }

    std::vector< std::string > OrdinalSampleSpace::get_ordered() const
    {
        std::vector< std::string > values(get_cardinality());
        for(std::set< std::string >::const_iterator it = _values.cbegin(), ite = _values.cend(); it != ite; ++it)
        { values[_rank[distance(_values.cbegin(), it)]] = *it; }
        return values;
    }

    void OrdinalSampleSpace::set_ordered(const std::vector< std::string >& ordered)
    {
        if(ordered.size() != _values.size())
        { throw std::runtime_error("rank"); }
        std::vector< Index > rank(ordered.size(), ordered.size());
        for(Index size = 0, max_size = ordered.size(); size < max_size; ++size)
        {
            std::set< std::string >::iterator it = _values.find(ordered[size]);
            if(it == _values.end())
            { throw std::runtime_error("rank"); }
            rank[distance(_values.begin(), it)] = size;
        }
        for(Index size = 0, max_size = ordered.size(); size < max_size; ++size)
        {
            if(rank[size] >= ordered.size())
            { throw std::runtime_error("ordered"); }
        }
        _rank = rank;
    }
    
    const std::vector< Index >& OrdinalSampleSpace::get_rank() const
    { return _rank; }

    void OrdinalSampleSpace::set_rank(const std::vector< Index >& rank)
    {
        if(rank.size() != _values.size())
        { throw std::runtime_error("rank"); }
        Indices order = Indices();
        for(Index size = 0, max_size = _values.size(); size < max_size; ++size)
        { order.insert(order.end(), size); }
        for(Index size = 0, max_size = _values.size(); size < max_size; ++size)
        {
            Indices::iterator it = order.find(rank[size]);
            if(it == order.end())
            { throw std::runtime_error("rank"); }
            order.erase(it);
        }
        if(order.size() != 0)
        { throw std::runtime_error("rank"); }
        _rank = rank;
    }

    void OrdinalSampleSpace::randomize()
    {
        std::set< std::string >::iterator ita = _values.begin(), ite = _values.end();
        ++ita;
        while(ita != ite)
        {
            std::set< std::string >::iterator itb = _values.begin();
            boost::random::uniform_int_distribution<> dist(0, distance(_values.begin(), ita));
            boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<>  > simulator(__impl::get_random_generator(), dist);
            advance(itb, simulator());
            Index buffer = _rank[distance(_values.cbegin(), ita)];
            _rank[distance(_values.cbegin(), ita)] = _rank[distance(_values.cbegin(), itb)];
            _rank[distance(_values.cbegin(), itb)] = buffer;
            ++ita;
        }
    }

    std::unique_ptr< UnivariateSampleSpace > OrdinalSampleSpace::copy() const
    { return std::make_unique< OrdinalSampleSpace >(*this); }

    std::unique_ptr< NominalSampleSpace > OrdinalSampleSpace::as_nominal() const
    { return std::make_unique< NominalSampleSpace >(_values); }


    HierarchicalSampleSpace::HierarchicalSampleSpace(const CategoricalSampleSpace& root_sample_space) : CategoricalSampleSpace(root_sample_space.get_values())
    {
        _tree_sample_space[""] = static_cast< CategoricalSampleSpace* >(root_sample_space.copy().release()); 
        for(std::set< std::string >::const_iterator it = root_sample_space.get_values().cbegin(), it_end = root_sample_space.get_values().cend(); it != it_end; ++it)
        { _parents[*it] = ""; }
    }

    HierarchicalSampleSpace::HierarchicalSampleSpace(const HierarchicalSampleSpace& p_sample_space) : CategoricalSampleSpace(p_sample_space.get_values())
    { 
        //_tree_sample_space = p_sample_space._tree_sample_space;
        _tree_sample_space.clear();
        for(std::map< std::string, CategoricalSampleSpace* >::const_iterator it = p_sample_space.cbegin(), it_end = p_sample_space.cend(); it != it_end; ++it)
        { _tree_sample_space[it->first] = static_cast< CategoricalSampleSpace* >(it->second->copy().release());; }
        _parents = p_sample_space._parents;
    }

    HierarchicalSampleSpace::~HierarchicalSampleSpace()
    { 
        for(std::map< std::string, CategoricalSampleSpace* >::iterator it = _tree_sample_space.begin(), it_end = _tree_sample_space.end(); it != it_end; ++it)
        { delete it->second; }
        _tree_sample_space.clear();
    }

    ordering_type HierarchicalSampleSpace::get_ordering() const
    {
        // std::map< std::string, CategoricalSampleSpace* >::const_iterator it = _tree_sample_space.cbegin(), it_end = _tree_sample_space.cend();
        // ordering_type ordering = it->second->get_ordering();
        // if(ordering != PARTIAL)
        // {
        //     ++it;
        //     while(it != it_end && ordering == it->second->get_ordering())
        //     { ++it; }
        // }
        // if(it != it_end)
        // { ordering = PARTIAL; }
        // return ordering;
        return PARTIAL;
    }

    void HierarchicalSampleSpace::set_encoding(const encoding_type& encoding)
    {}

    Eigen::RowVectorXd HierarchicalSampleSpace::encode(const std::string& value) const
    {
        Eigen::RowVectorXd dummy; // TODO
        throw not_implemented_error("encode");
        return dummy;
    }

    void HierarchicalSampleSpace::partition(const std::string& leave, const CategoricalSampleSpace& sample_space)
    {
        if(CategoricalSampleSpace::is_compatible_value(leave))
        {  
            const std::set< std::string >& values = sample_space.get_values();
            std::set< std::string >::const_iterator it = values.cbegin(), it_end = values.cend();
            while(it != it_end && !is_compatible_value(*it))
            { ++it; }
            if(it == it_end)
            {
                _values.erase(leave);
                _values.insert(values.cbegin(), values.cend());
                _tree_sample_space[leave] = static_cast< CategoricalSampleSpace* >(sample_space.copy().release()); 
                for(std::set< std::string >::const_iterator it = sample_space.get_values().cbegin(), it_end = sample_space.get_values().cend(); it != it_end; ++it)
                { _parents[*it] = leave; }                
            }    
            else
            { throw in_set_error("leave", *it, __impl::keys(_tree_sample_space)); }
        }    
        else
        { throw in_set_error("leave", leave, _values, false); }
    }

    UnivariateConditionalData HierarchicalSampleSpace::split(const std::string& non_leave, const UnivariateConditionalData& data) const
    {
        MultivariateDataFrame explanatories_data(*(data.get_explanatories()->get_sample_space()));
        UnivariateDataFrame response_data(*((_tree_sample_space.find(non_leave))->second));

        std::map< std::string, std::string > new_leaves;
        for(std::set< std::string >::const_iterator it = _values.begin(), it_end = _values.cend(); it != it_end; ++it)
        { new_leaves[*it] = children(non_leave, *it); }

        std::unique_ptr< UnivariateConditionalData::Generator > generator = data.generator();
        std::vector< double > weights;
        while(generator->is_valid())
        {
            const CategoricalElementaryEvent* response_event = static_cast< const CategoricalElementaryEvent* >(generator->response()->copy().release());
            if(response_event)
            {
                std::string new_response = new_leaves.find(response_event->get_value())->second;
                if(new_response != "")
                {
                    CategoricalElementaryEvent* new_response_event = new CategoricalElementaryEvent(new_response);
                    response_data.add_event(new_response_event);
                    delete new_response_event;
                    weights.push_back(generator->weight());
                    explanatories_data.add_event(generator->explanatories());
                }
            }
            ++(*generator);
        }
        UnivariateDataFrame* resp_data = new UnivariateDataFrame(response_data);
        WeightedUnivariateData weighted_response_data(resp_data, weights);
        delete resp_data;
        UnivariateConditionalData new_data(weighted_response_data, explanatories_data);
        return new_data;
    }

    std::string HierarchicalSampleSpace::children(const std::string& non_leave, const std::string& leave) const
    {
        std::map< std::string, std::string >::const_iterator it_par = _parents.find(leave);
        std::map< std::string, CategoricalSampleSpace* >::const_iterator it = _tree_sample_space.find(non_leave);
        if(it_par != _parents.cend())
        {
            if(it != _tree_sample_space.cend())
            {
                while(it_par->second != "" && it_par->second != non_leave)
                { it_par = _parents.find(it_par->second); }
            }
            else
            { throw in_set_error("non-leave", non_leave, __impl::keys(_tree_sample_space), false); }
        }
        else
        { throw in_set_error("leave", leave, __impl::keys(_parents), false); }
        if(it_par->second == non_leave)
        { return it_par->first; }
        else
        { return ""; }
    }

    std::unique_ptr< UnivariateSampleSpace > HierarchicalSampleSpace::copy() const
    { return std::make_unique< HierarchicalSampleSpace >(*this); }

    HierarchicalSampleSpace::const_iterator HierarchicalSampleSpace::cbegin() const
    { return _tree_sample_space.cbegin(); }

    HierarchicalSampleSpace::const_iterator HierarchicalSampleSpace::cend() const
    { return _tree_sample_space.cend(); }
            
    const CategoricalSampleSpace* HierarchicalSampleSpace::get_sample_space(const std::string& value)
    { return _tree_sample_space[value]; }

    std::map< std::string, std::string > HierarchicalSampleSpace::get_parents() const
    { return _parents; }

    // const std::string HierarchicalSampleSpace::get_parent(const std::string& value)
    // {
    //     std::map< std::string, std::string >::const_iterator it = _parents.find(value);
    //     if(it != _parents.cend())
    //     { return _parents.find(value)->second; }
    //     else
    //     { throw in_set_error("value", value, __impl::keys(_parents)); }
    // }

    bool HierarchicalSampleSpace::is_compatible_value(const std::string& value) const
    {
        bool compatible = CategoricalSampleSpace::is_compatible_value(value);
        if(!compatible)
        {
            if(value != "")
            { compatible = (_tree_sample_space.find(value) != _tree_sample_space.end()); }
        }
        return compatible;
    }

    outcome_type DiscreteSampleSpace::get_outcome() const
    { return DISCRETE; }

    ordering_type DiscreteSampleSpace::get_ordering() const
    { return TOTAL; }

    IntegerSampleSpace::IntegerSampleSpace(const int& lower_bound, const int& upper_bound)
    {
        _lower_bound = lower_bound;
        _upper_bound = upper_bound;
    }

    IntegerSampleSpace::~IntegerSampleSpace()
    {}

    bool IntegerSampleSpace::is_compatible(const UnivariateEvent* event) const
    {
        bool compatible = !event;
        if(!compatible)
        {
            if(event->get_outcome() == DISCRETE)
            {
                switch(event->get_event())
                {
                    case ELEMENTARY:
                        {
                            int value = static_cast< const DiscreteElementaryEvent* >(event)->get_value();
                            compatible = value >= _lower_bound && value <= _upper_bound;
                        }    
                        break;
                    case CENSORED:
                        {
                            const std::vector< int >& __values = static_cast< const DiscreteCensoredEvent* >(event)->get_values();
                            std::vector< int >::const_iterator it = __values.cbegin(), ite = __values.cend();
                            compatible = true;
                            while(compatible && it != ite)
                            {
                                compatible = *it >= _lower_bound && *it <= _upper_bound;
                                ++it;
                            }
                        }
                        break;
                    case LEFT:
                        if(_lower_bound == std::numeric_limits< int >::min())
                        {
                            int value = static_cast< const DiscreteLeftCensoredEvent* >(event)->get_upper_bound();
                            compatible = value >= _lower_bound && value <= _upper_bound;
                        }
                        else
                        { compatible = false; }
                        break;
                    case RIGHT:
                        if(_upper_bound == std::numeric_limits< int >::max())
                        {
                            int value = static_cast< const DiscreteRightCensoredEvent* >(event)->get_lower_bound();
                            compatible = value >= _lower_bound && value <= _upper_bound;
                        }
                        else
                        { compatible = false; }
                        break;
                    case INTERVAL:
                        {
                            int value = static_cast< const DiscreteIntervalCensoredEvent* >(event)->get_upper_bound();
                            compatible = value >= _lower_bound && value <= _upper_bound;
                            if(compatible)
                            {
                                value = static_cast< const DiscreteIntervalCensoredEvent* >(event)->get_upper_bound();
                                compatible = value >= _lower_bound && value <= _upper_bound;
                            }
                        }
                        break;
                }
            }
        }
        return compatible;
    }

    const int& IntegerSampleSpace::get_lower_bound() const
    { return _lower_bound; }

    const int& IntegerSampleSpace::get_upper_bound() const
    { return _upper_bound; }

    std::unique_ptr< UnivariateSampleSpace > IntegerSampleSpace::copy() const
    { return std::make_unique< IntegerSampleSpace >(*this); }

    const IntegerSampleSpace NN = IntegerSampleSpace(0);

    const IntegerSampleSpace& get_NN()
    { return NN; }

    const IntegerSampleSpace ZZ = IntegerSampleSpace();

    const IntegerSampleSpace& get_ZZ()
    { return ZZ; }

    outcome_type ContinuousSampleSpace::get_outcome() const
    { return CONTINUOUS; }

    ordering_type ContinuousSampleSpace::get_ordering() const
    { return TOTAL; }

    RealSampleSpace::RealSampleSpace(const double& lhs, const double& rhs, const bool& left_closed, const bool& right_closed)
    {
        if(lhs < rhs)
        {
            _lower_bound = lhs;
            _upper_bound = rhs;
        }
        else
        {
            _lower_bound = rhs;
            _upper_bound = lhs;
        }
        _left_closed = left_closed && !boost::math::isinf(_lower_bound);
        _right_closed = right_closed && !boost::math::isinf(_upper_bound);
    }

    RealSampleSpace::~RealSampleSpace()
    {}

    bool RealSampleSpace::is_compatible(const UnivariateEvent* event) const
    {
        bool compatible = !event;
        if(!compatible)
        {
            if(event->get_outcome() == CONTINUOUS)
            {
                switch(event->get_event())
                {
                    case ELEMENTARY:
                        {
                            double value = static_cast< const ContinuousElementaryEvent* >(event)->get_value();
                            compatible = boost::math::isfinite(value);
                            if(compatible)
                            {
                                if(_left_closed)
                                { compatible = value >= _lower_bound; }
                                else
                                { compatible = value > _lower_bound; }
                                if(compatible)
                                {
                                    if(_right_closed)
                                    { compatible = value <= _upper_bound; }
                                    else
                                    { compatible = value < _upper_bound; }
                                }
                            }
                        }    
                        break;
                    case CENSORED:
                        {
                            const std::vector< double >& __values = static_cast< const ContinuousCensoredEvent* >(event)->get_values();
                            std::vector< double >::const_iterator it = __values.cbegin(), ite = __values.cend();
                            compatible = true;
                            while(compatible && it != ite)
                            {
                                compatible = boost::math::isfinite(*it);
                                if(compatible)
                                {
                                    if(_left_closed)
                                    { compatible = *it >= _lower_bound; }
                                    else
                                    { compatible = *it > _lower_bound; }
                                    if(compatible)
                                    {
                                        if(_right_closed)
                                        { compatible = *it <= _upper_bound; }
                                        else
                                        { compatible = *it < _upper_bound; }
                                    }
                                }
                                ++it;
                            }
                        }
                        break;
                    case LEFT:
                        if(boost::math::isinf(_lower_bound) && _lower_bound < 0)
                        {
                            double value = static_cast< const ContinuousLeftCensoredEvent* >(event)->get_upper_bound();
                            compatible = boost::math::isfinite(value) && value >= _lower_bound && value <= _upper_bound;
                        }
                        else
                        { compatible = false; }
                        break;
                    case RIGHT:
                        if(boost::math::isinf(_upper_bound) && _upper_bound > 0)
                        {
                            double value = static_cast< const ContinuousRightCensoredEvent* >(event)->get_lower_bound();
                            compatible = boost::math::isfinite(value) && value >= _lower_bound && value <= _upper_bound;
                        }
                        else
                        { compatible = false; }
                        break;
                    case INTERVAL:
                        {
                            double value = static_cast< const ContinuousIntervalCensoredEvent* >(event)->get_upper_bound();
                        compatible = boost::math::isfinite(value) && value >= _lower_bound && value <= _upper_bound;
                            if(compatible)
                            {
                                value = static_cast< const ContinuousIntervalCensoredEvent* >(event)->get_upper_bound();
                                compatible = boost::math::isfinite(value) && value >= _lower_bound && value <= _upper_bound;
                            }
                        }
                        break;
                }
            }
        }
        return compatible;
    }

    const double& RealSampleSpace::get_lower_bound() const
    { return _lower_bound; }

    const double& RealSampleSpace::get_upper_bound() const
    { return _upper_bound; }

    const bool& RealSampleSpace::get_left_closed() const
    { return _left_closed; }

    const bool& RealSampleSpace::get_right_closed() const
    { return _right_closed; }
    
    std::unique_ptr< UnivariateSampleSpace > RealSampleSpace::copy() const
    { return std::make_unique< RealSampleSpace >(*this); }

    const RealSampleSpace RR = RealSampleSpace();

    const RealSampleSpace& get_RR()
    { return RR; }

    const RealSampleSpace PR = RealSampleSpace(0);

    const RealSampleSpace& get_PR()
    { return PR; }

    const RealSampleSpace NR = RealSampleSpace(-1*std::numeric_limits< double >::infinity(), 0);

    const RealSampleSpace& get_NR()
    { return NR; }

    /*Eigen::MatrixXd MultivariateSampleSpace::encode(const MultivariateEvent& event, const std::set< std::set<Index> >& interactions) const
    {
        Eigen::MatrixXd dummy = Eigen::MatrixXd::Zero(1, compute_encode_length(interactions));
        Index dindex = 0;
        for(Index index = 0, max_index = size(); index < max_index; ++index)
        {
            if(_sample_spaces[index]->get_outcome() == CATEGORICAL)
            {
                Eigen::MatrixXd tdummy;
                if(event[index].get_event() == ELEMENTARY)
                { tdummy = boost::static_pointer_cast< CategoricalSampleSpace >(_sample_spaces[index])->encode(static_cast< const CategoricalElementaryEvent& >(event[index]).get_outcome()) ; }
                else
                { 
                    Index cardinality = boost::static_pointer_cast< CategoricalSampleSpace >(_sample_spaces[index])->get_cardinality();
                    if(cardinality > 1)
                    { tdummy = Eigen::MatrixXd::Constant(1, cardinality-1, NAN) ; }
                    else
                    { tdummy = Eigen::MatrixXd::Zero(1,0); }
                }
                dummy.block(0, dindex, 1, tdummy.cols()) = tdummy;
                dindex += tdummy.cols();
            }
            else
            {
                if(event[index].get_event() == ELEMENTARY)
                {
                    if(event[index].get_outcome() == DISCRETE)
                    { dummy(0, index) = static_cast< const DiscreteElementaryEvent& >(event[index]).get_outcome(); }
                    else if(event[index].get_outcome() == CONTINUOUS)
                    { dummy(0, index) = static_cast< const ContinuousElementaryEvent& >(event[index]).get_outcome(); }
                    else
                    { dummy(0, index) = NAN; }
                }
                else
                { dummy(0, dindex) = NAN; }
                ++dindex;
            }
        }
        for(std::set< std::set<Index> >::const_iterator it = interactions.cbegin(), ite = interactions.cend(); it != ite; ++it)
        {
            for(std::set<Index>::const_iterator itb = it->cbegin(), itbe = it->cend(); itb != itbe; ++itb)
            {
                Eigen::MatrixXd tdummy = Eigen::MatrixXd::Ones(1,1);
                if(_sample_spaces[*itb]->get_outcome() == CATEGORICAL)
                {
                    if(event[*itb].get_event() == ELEMENTARY)
                    { tdummy = Eigen::kroneckerProduct(tdummy, boost::static_pointer_cast< CategoricalSampleSpace >(_sample_spaces[*itb])->encode(static_cast< const CategoricalElementaryEvent& >(event[*itb]).get_outcome())).eval(); }
                    else
                    { 
                        Index cardinality = boost::static_pointer_cast< CategoricalSampleSpace >(_sample_spaces[*itb])->get_cardinality();
                        if(cardinality > 1)
                        { tdummy = Eigen::kroneckerProduct(tdummy, Eigen::MatrixXd::Constant(1, cardinality-1, NAN)).eval(); }
                        else
                        { tdummy = Eigen::MatrixXd::Zero(1,0); }
                    }
                }
                else
                {
                    if(event[*itb].get_event() == ELEMENTARY)
                    {
                        if(event[*itb].get_outcome() == DISCRETE)
                        { tdummy *= static_cast< const DiscreteElementaryEvent& >(event[*itb]).get_outcome(); }
                        else if(event[*itb].get_outcome() == CONTINUOUS)
                        { tdummy *= static_cast< const ContinuousElementaryEvent& >(event[*itb]).get_outcome(); }
                        else
                        { tdummy *= NAN; }
                    }
                    else
                    { tdummy *= NAN; }
                }
                dummy.block(0, dindex, 1, tdummy.cols()) = tdummy;
                dindex += tdummy.cols();
            }
        }
        return dummy;
    }*/

    /*MultivariateSampleSpace::MultivariateSampleSpace(const std::vector< std::shared_ptr< UnivariateSampleSpace > >& sample_spaces)
    { _sample_spaces = sample_spaces; }

    MultivariateSampleSpace::MultivariateSampleSpace(const MultivariateSampleSpace& sample_space)
    {
        _sample_spaces.resize(sample_space.size());
        for(Index index = 0, max_index = sample_space.size(); index < max_index; ++index)
        {
            std::shared_ptr< UnivariateSampleSpace > marginal_sample_space = sample_space.get_sample_space(index);
            if(marginal_sample_space)
            {
                if(marginal_sample_space->get_outcome() == CATEGORICAL)
                { _sample_spaces[index] = std::shared_ptr< UnivariateSampleSpace >(marginal_sample_space->copy()); }
                else
                { _sample_spaces[index] = marginal_sample_space; }
            }
        }
    }*/

    MultivariateSampleSpace::~MultivariateSampleSpace()
    {}

    bool MultivariateSampleSpace::is_compatible(const MultivariateEvent* event) const
    {
        bool compatible = !event || event->size() == size();
        if(compatible)
        {
            const UnivariateSampleSpace* sample_space;
            Index index = 0, max_index = size();
            while(compatible && index < max_index)
            {
                sample_space = get(index);
                compatible = sample_space && sample_space->is_compatible(event->get(index));
                ++index;
            }
        }
        else
        { compatible = event; }
        return compatible;
    }

    Index MultivariateSampleSpace::encode() const
    {
        Index _size = 0;
        for(Index index = 0, max_index = size(); index < max_index; ++index)
        {
            const UnivariateSampleSpace* sample_space = get(index);
            if(sample_space->get_outcome() == CATEGORICAL)
            {
                _size += static_cast< const CategoricalSampleSpace* >(sample_space)->get_cardinality();
                _size -= 1;
            }
            else
            { _size += 1; }
        }
        return _size;
    }

    Eigen::RowVectorXd MultivariateSampleSpace::encode(const MultivariateEvent& event) const
    {
        Eigen::RowVectorXd dummy;
        if(event.size() != size())
        { dummy = std::numeric_limits< double >::quiet_NaN() * Eigen::RowVectorXd::Ones(encode()); }
        else
        {
            Index shift = 0;
            dummy = Eigen::RowVectorXd::Zero(encode());
            Eigen::RowVectorXd temp;
            for(Index index = 0, max_index = size(); index< max_index; ++index)
            {
                const UnivariateEvent* uevent = event.get(index);
                if(uevent->get_event() == ELEMENTARY)
                {
                    const UnivariateSampleSpace* sample_space = get(index);
                    switch(sample_space->get_outcome())
                    {
                        case CATEGORICAL:
                            {
                                temp = (static_cast< const CategoricalSampleSpace* >(sample_space)->encode(static_cast< const CategoricalElementaryEvent* >(uevent)->get_value()));
                                dummy.segment(index + shift, temp.size()) = temp;
                                shift += temp.size();
                                --shift;
                            }
                            break;
                        case DISCRETE:
                            dummy(index + shift) = static_cast< const DiscreteElementaryEvent* >(uevent)->get_value();
                            break;
                        case CONTINUOUS:
                            dummy(index + shift) = static_cast< const ContinuousElementaryEvent* >(uevent)->get_value();
                            break;
                    }
                }
                else
                {
                    const UnivariateSampleSpace* sample_space = get(index);
                    if(sample_space->get_outcome() == CATEGORICAL)
                    {
                        Index max_size = index + shift + static_cast< const CategoricalSampleSpace* >(sample_space)->get_cardinality();
                        while(index + shift < max_size)
                        {
                            dummy(index + shift) = std::numeric_limits< double >::quiet_NaN();
                            ++shift;
                        }
                    }
                    else
                    { dummy(index + shift) = std::numeric_limits< double >::quiet_NaN(); }
                }
            }
        }
        return dummy;
    }

    VectorSampleSpace::VectorSampleSpace(const std::vector< UnivariateSampleSpace* >& sample_spaces)
    {
        _sample_spaces.resize(sample_spaces.size(), nullptr);
        for(Index index = 0, max_index = sample_spaces.size(); index < max_index; ++index)
        {
            if(!sample_spaces[index])
            { throw nullptr_error("sample_spaces"); }
            else
            { _sample_spaces[index] = sample_spaces[index]->copy().release(); }
        }
    }

    VectorSampleSpace::VectorSampleSpace(const VectorSampleSpace& sample_space)
    {
        _sample_spaces.resize(sample_space.size(), nullptr);
        for(Index index = 0, max_index = sample_space.size(); index < max_index; ++index)
        { _sample_spaces[index] = sample_space._sample_spaces[index]->copy().release(); }
    }

    VectorSampleSpace::~VectorSampleSpace()
    {
        for(Index index = 0, max_index = _sample_spaces.size(); index < max_index; ++index)
        {
            if(_sample_spaces[index])
            { delete _sample_spaces[index]; }
            _sample_spaces[index] = nullptr; 
        }
        _sample_spaces.clear();
    }

    Index VectorSampleSpace::size() const
    {return _sample_spaces.size(); }

    const UnivariateSampleSpace* VectorSampleSpace::get(const Index& index) const
    { return _sample_spaces[index]; }

    void VectorSampleSpace::set(const Index& index, const UnivariateSampleSpace& sample_space)
    { 
        delete _sample_spaces[index];
        _sample_spaces[index] = sample_space.copy().release();
    }
    
    std::unique_ptr< MultivariateSampleSpace > VectorSampleSpace::copy() const
    { return std::make_unique< VectorSampleSpace >(*this); }

}
