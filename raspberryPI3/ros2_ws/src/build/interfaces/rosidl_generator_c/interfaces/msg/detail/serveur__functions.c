// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from interfaces:msg/Serveur.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/serveur__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
interfaces__msg__Serveur__init(interfaces__msg__Serveur * msg)
{
  if (!msg) {
    return false;
  }
  // departure_point
  msg->departure_point = 65;
  // final_point
  msg->final_point = 65;
  // request_number
  msg->request_number = 0;
  return true;
}

void
interfaces__msg__Serveur__fini(interfaces__msg__Serveur * msg)
{
  if (!msg) {
    return;
  }
  // departure_point
  // final_point
  // request_number
}

bool
interfaces__msg__Serveur__are_equal(const interfaces__msg__Serveur * lhs, const interfaces__msg__Serveur * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // departure_point
  if (lhs->departure_point != rhs->departure_point) {
    return false;
  }
  // final_point
  if (lhs->final_point != rhs->final_point) {
    return false;
  }
  // request_number
  if (lhs->request_number != rhs->request_number) {
    return false;
  }
  return true;
}

bool
interfaces__msg__Serveur__copy(
  const interfaces__msg__Serveur * input,
  interfaces__msg__Serveur * output)
{
  if (!input || !output) {
    return false;
  }
  // departure_point
  output->departure_point = input->departure_point;
  // final_point
  output->final_point = input->final_point;
  // request_number
  output->request_number = input->request_number;
  return true;
}

interfaces__msg__Serveur *
interfaces__msg__Serveur__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__Serveur * msg = (interfaces__msg__Serveur *)allocator.allocate(sizeof(interfaces__msg__Serveur), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(interfaces__msg__Serveur));
  bool success = interfaces__msg__Serveur__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
interfaces__msg__Serveur__destroy(interfaces__msg__Serveur * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    interfaces__msg__Serveur__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
interfaces__msg__Serveur__Sequence__init(interfaces__msg__Serveur__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__Serveur * data = NULL;

  if (size) {
    data = (interfaces__msg__Serveur *)allocator.zero_allocate(size, sizeof(interfaces__msg__Serveur), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = interfaces__msg__Serveur__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        interfaces__msg__Serveur__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
interfaces__msg__Serveur__Sequence__fini(interfaces__msg__Serveur__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      interfaces__msg__Serveur__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

interfaces__msg__Serveur__Sequence *
interfaces__msg__Serveur__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__Serveur__Sequence * array = (interfaces__msg__Serveur__Sequence *)allocator.allocate(sizeof(interfaces__msg__Serveur__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = interfaces__msg__Serveur__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
interfaces__msg__Serveur__Sequence__destroy(interfaces__msg__Serveur__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    interfaces__msg__Serveur__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
interfaces__msg__Serveur__Sequence__are_equal(const interfaces__msg__Serveur__Sequence * lhs, const interfaces__msg__Serveur__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!interfaces__msg__Serveur__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
interfaces__msg__Serveur__Sequence__copy(
  const interfaces__msg__Serveur__Sequence * input,
  interfaces__msg__Serveur__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(interfaces__msg__Serveur);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    interfaces__msg__Serveur * data =
      (interfaces__msg__Serveur *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!interfaces__msg__Serveur__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          interfaces__msg__Serveur__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!interfaces__msg__Serveur__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
