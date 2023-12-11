// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/Serveur.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__SERVEUR__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__SERVEUR__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__Serveur __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__Serveur __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Serveur_
{
  using Type = Serveur_<ContainerAllocator>;

  explicit Serveur_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->departure_point = 65;
      this->final_point = 65;
      this->request_number = 0;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->departure_point = 0;
      this->final_point = 0;
      this->request_number = 0;
    }
  }

  explicit Serveur_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->departure_point = 65;
      this->final_point = 65;
      this->request_number = 0;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->departure_point = 0;
      this->final_point = 0;
      this->request_number = 0;
    }
  }

  // field types and members
  using _departure_point_type =
    uint8_t;
  _departure_point_type departure_point;
  using _final_point_type =
    uint8_t;
  _final_point_type final_point;
  using _request_number_type =
    int8_t;
  _request_number_type request_number;

  // setters for named parameter idiom
  Type & set__departure_point(
    const uint8_t & _arg)
  {
    this->departure_point = _arg;
    return *this;
  }
  Type & set__final_point(
    const uint8_t & _arg)
  {
    this->final_point = _arg;
    return *this;
  }
  Type & set__request_number(
    const int8_t & _arg)
  {
    this->request_number = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::Serveur_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::Serveur_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::Serveur_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::Serveur_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::Serveur_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::Serveur_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::Serveur_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::Serveur_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::Serveur_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::Serveur_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__Serveur
    std::shared_ptr<interfaces::msg::Serveur_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__Serveur
    std::shared_ptr<interfaces::msg::Serveur_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Serveur_ & other) const
  {
    if (this->departure_point != other.departure_point) {
      return false;
    }
    if (this->final_point != other.final_point) {
      return false;
    }
    if (this->request_number != other.request_number) {
      return false;
    }
    return true;
  }
  bool operator!=(const Serveur_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Serveur_

// alias to use template instance with default allocator
using Serveur =
  interfaces::msg::Serveur_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__SERVEUR__STRUCT_HPP_
