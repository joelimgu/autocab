// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/Serveur.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__SERVEUR__STRUCT_H_
#define INTERFACES__MSG__DETAIL__SERVEUR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/Serveur in the package interfaces.
typedef struct interfaces__msg__Serveur
{
  uint8_t departure_point;
  uint8_t final_point;
  int8_t request_number;
} interfaces__msg__Serveur;

// Struct for a sequence of interfaces__msg__Serveur.
typedef struct interfaces__msg__Serveur__Sequence
{
  interfaces__msg__Serveur * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__Serveur__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__SERVEUR__STRUCT_H_
