#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/stl_bind.h>
#include <iostream>

namespace py = pybind11;

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

struct process_subobject
{
    inline void
    operator()(const subobject& o) const
    {
        std::cout << "cpp callable doing the job!\n";
        std::cout << o.x << '\n';
    }
};

void
doit(const object& o, py::function sf_, py::function of_)
{
    std::cout << "work on object:\n";
    of_(py::cast(o, py::return_value_policy::reference));
    std::cout << "done\nwork on subobject:\n";
    sf_(py::cast(o.s, py::return_value_policy::reference));
    std::cout << "done\n";
}

PYBIND11_PLUGIN(copying3)
{
    py::module m("copying3", "expensive copying");

    py::class_<subobject>(m, "Subobject")
        .def(py::init<int>())
        .def_readonly("x", &subobject::x);

    py::class_<object>(m, "Object")
        .def(py::init<int>())
        .def_readonly("s", &object::s);

    // This appears automagically castable
    // to py::function
    py::class_<process_subobject>(m, "ProcessSubobject")
        .def(py::init<>())
        .def("__call__", [](const process_subobject& po,
                            const subobject& s) -> void { return po(s); });

    m.def("doit", &doit);

    return m.ptr();
}
