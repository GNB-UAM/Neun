/*************************************************************

*************************************************************/

#ifndef NORMALIZABLESYNAPSECONCEPT_H_
#define NORMALIZABLESYNAPSECONCEPT_H_

#include <concepts>
#include <type_traits>

/*
 *  \class NormalizableSynapseConcept
 *
 *  A normalizable synapse is a synapse that can be normalized.
 *
 *  A model of this concept must implement the following methods:
 *  \li precission_t get_w_max() const
 *  \li precission_t get_weight() const
 *  \li void set_weight(precission_t value)
 */
template <typename T>
concept NormalizableSynapseConcept = requires(T s, const T t) {
    typename T::precission_t;
    { s.set_weight(typename T::precission_t{}) } -> std::same_as<void>;
    { t.get_w_max() } -> std::convertible_to<typename T::precission_t>;
    { t.get_weight() } -> std::convertible_to<typename T::precission_t>;
};

#endif /*NORMALIZABLESYNAPSECONCEPT_H_*/
