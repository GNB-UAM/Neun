#ifndef IZHIKEVICHSYSTEMWRAPPER_H_
#define IZHIKEVICHSYSTEMWRAPPER_H_

#include "SystemWrapper.h"
#include "IzhikevichModel.h"

template <typename Precision>
class IzhikevichSystemWrapper : public SystemWrapper<IzhikevichModel<Precision>> {
public:
    using Base = SystemWrapper<IzhikevichModel<Precision>>;
    using typename Base::precision_t;
    using typename Base::variable;
    using typename Base::parameter;

    void pre_step(precision_t /*h*/) {
    }

    void post_step(precision_t /*h*/) {
        // Izhikevich reset logic: if v >= threshold, reset v and update u
        if (this->get(variable::v) >= this->get(parameter::threshold)) {
            this->set(variable::v, this->get(parameter::c));
            this->set(variable::u, this->get(variable::u) + this->get(parameter::d));
        }
    }
};

#endif /* IZHIKEVICHSYSTEMWRAPPER_H_ */