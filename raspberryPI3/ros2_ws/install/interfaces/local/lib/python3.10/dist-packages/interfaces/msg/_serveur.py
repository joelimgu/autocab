# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/Serveur.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_Serveur(type):
    """Metaclass of message 'Serveur'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'interfaces.msg.Serveur')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__serveur
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__serveur
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__serveur
            cls._TYPE_SUPPORT = module.type_support_msg__msg__serveur
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__serveur

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
            'DEPARTURE_POINT__DEFAULT': 65,
            'FINAL_POINT__DEFAULT': 65,
            'REQUEST_NUMBER__DEFAULT': 0,
        }

    @property
    def DEPARTURE_POINT__DEFAULT(cls):
        """Return default value for message field 'departure_point'."""
        return 65

    @property
    def FINAL_POINT__DEFAULT(cls):
        """Return default value for message field 'final_point'."""
        return 65

    @property
    def REQUEST_NUMBER__DEFAULT(cls):
        """Return default value for message field 'request_number'."""
        return 0


class Serveur(metaclass=Metaclass_Serveur):
    """Message class 'Serveur'."""

    __slots__ = [
        '_departure_point',
        '_final_point',
        '_request_number',
    ]

    _fields_and_field_types = {
        'departure_point': 'uint8',
        'final_point': 'uint8',
        'request_number': 'int8',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('int8'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.departure_point = kwargs.get(
            'departure_point', Serveur.DEPARTURE_POINT__DEFAULT)
        self.final_point = kwargs.get(
            'final_point', Serveur.FINAL_POINT__DEFAULT)
        self.request_number = kwargs.get(
            'request_number', Serveur.REQUEST_NUMBER__DEFAULT)

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.departure_point != other.departure_point:
            return False
        if self.final_point != other.final_point:
            return False
        if self.request_number != other.request_number:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def departure_point(self):
        """Message field 'departure_point'."""
        return self._departure_point

    @departure_point.setter
    def departure_point(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'departure_point' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'departure_point' field must be an unsigned integer in [0, 255]"
        self._departure_point = value

    @builtins.property
    def final_point(self):
        """Message field 'final_point'."""
        return self._final_point

    @final_point.setter
    def final_point(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'final_point' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'final_point' field must be an unsigned integer in [0, 255]"
        self._final_point = value

    @builtins.property
    def request_number(self):
        """Message field 'request_number'."""
        return self._request_number

    @request_number.setter
    def request_number(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'request_number' field must be of type 'int'"
            assert value >= -128 and value < 128, \
                "The 'request_number' field must be an integer in [-128, 127]"
        self._request_number = value
