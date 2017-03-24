#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/stl_bind.h>
#include <iostream>

struct subobject
{
    int x;
    subobject(int x_) : x(x_) {}
    subobject(const subobject& s) : x(s.x)
    {
        std::cout << "subobject copied\n";
    }
};

struct object
{
    subobject s;
    object(int x) : s(x) {}
    object(const object& o) : s(o.s) { std::cout << "object copied\n"; }
};

using sf = std::function<void(const subobject&)>;
using of = std::function<void(const object&)>;

void
doit(const object& o, sf sf_, of of_)
{
    std::cout << "work on object:\n";
    of_(o);
    std::cout << "done\nwork on subobject:\n";
    sf_(o.s);
    std::cout << "done\n";
}

namespace py = pybind11;

PYBIND11_PLUGIN(copying2)
{
    py::module m("copying2", "expensive copying");

    py::class_<subobject>(m, "Subobject")
        .def(py::init<int>())
        .def_readonly("x", &subobject::x);

    py::class_<object>(m, "Object")
        .def(py::init<int>())
        .def_readonly("s", &object::s);

    m.def("doit", &doit);

    return m.ptr();
}
