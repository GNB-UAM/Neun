#ifndef MODELBASE_H_
#define MODELBASE_H_

#ifndef __AVR_ARCH__
#include <type_traits>
#endif  //__AVR_ARCH__

template <typename Precision>
class ModelBase {
public:
    static_assert(std::is_arithmetic_v<Precision>, "Precision must be an arithmetic type");

	typedef Precision precision_t;

	enum variable {x, y, n_variables = 1};
	enum parameter {alpha, beta, n_parameters = 1};
	
	void eval(precision_t * const vars, precision_t * const params, precision_t *incs) const
	{
	}

	// pre_step and post_step are optional in Model concept
	void pre_step(precision_t h)
	{
		// Pre-step actions
	}
	void post_step(precision_t h)
	{
		// Post-step actions
	}
};

#endif /*MODELBASE_H_*/