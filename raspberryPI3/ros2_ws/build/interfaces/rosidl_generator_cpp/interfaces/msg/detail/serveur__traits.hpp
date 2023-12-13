// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from interfaces:msg/Serveur.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__SERVEUR__TRAITS_HPP_
#define INTERFACES__MSG__DETAIL__SERVEUR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "interfaces/msg/detail/serveur__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Serveur & msg,
  std::ostream & out)
{
  out << "{";
  // member: departure_point
  {
    out << "departure_point: ";
    rosidl_generator_traits::value_to_yaml(msg.departure_point, out);
    out << ", ";
  }

  // member: final_point
  {
    out << "final_point: ";
    rosidl_generator_traits::value_to_yaml(msg.final_point, out);
    out << ", ";
  }

  // member: request_number
  {
    out << "request_number: ";
    rosidl_generator_traits::value_to_yaml(msg.request_number, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Serveur & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: departure_point
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "departure_point: ";
    rosidl_generator_traits::value_to_yaml(msg.departure_point, out);
    out << "\n";
  }

  // member: final_point
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "final_point: ";
    rosidl_generator_traits::value_to_yaml(msg.final_point, out);
    out << "\n";
  }

  // member: request_number
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "request_number: ";
    rosidl_generator_traits::value_to_yaml(msg.request_number, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Serveur & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace interfaces

namespace rosidl_generator_traits
{

[[deprecated("use interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const interfaces::msg::Serveur & msg,
  std::ostream & out, size_t indentation = 0)
{
  interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const interfaces::msg::Serveur & msg)
{
  return interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<interfaces::msg::Serveur>()
{
  return "interfaces::msg::Serveur";
}

template<>
inline const char * name<interfaces::msg::Serveur>()
{
  return "interfaces/msg/Serveur";
}

template<>
struct has_fixed_size<interfaces::msg::Serveur>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<interfaces::msg::Serveur>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<interfaces::msg::Serveur>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // INTERFACES__MSG__DETAIL__SERVEUR__TRAITS_HPP_
