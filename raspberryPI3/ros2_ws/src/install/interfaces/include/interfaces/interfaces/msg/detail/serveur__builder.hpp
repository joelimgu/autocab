// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/Serveur.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__SERVEUR__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__SERVEUR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "interfaces/msg/detail/serveur__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_Serveur_request_number
{
public:
  explicit Init_Serveur_request_number(::interfaces::msg::Serveur & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::Serveur request_number(::interfaces::msg::Serveur::_request_number_type arg)
  {
    msg_.request_number = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::Serveur msg_;
};

class Init_Serveur_final_point
{
public:
  explicit Init_Serveur_final_point(::interfaces::msg::Serveur & msg)
  : msg_(msg)
  {}
  Init_Serveur_request_number final_point(::interfaces::msg::Serveur::_final_point_type arg)
  {
    msg_.final_point = std::move(arg);
    return Init_Serveur_request_number(msg_);
  }

private:
  ::interfaces::msg::Serveur msg_;
};

class Init_Serveur_departure_point
{
public:
  Init_Serveur_departure_point()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Serveur_final_point departure_point(::interfaces::msg::Serveur::_departure_point_type arg)
  {
    msg_.departure_point = std::move(arg);
    return Init_Serveur_final_point(msg_);
  }

private:
  ::interfaces::msg::Serveur msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::Serveur>()
{
  return interfaces::msg::builder::Init_Serveur_departure_point();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__SERVEUR__BUILDER_HPP_
