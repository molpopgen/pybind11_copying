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

template<typename sf_>
void external_lib_template_fxn(object & o,const sf_ sf)
{
    auto sc = o.s;
    sf(sc);
}

void
doit(object& o, py::function sf_, of of_)
{
    std::cout << "pass off to another lib\nthat works on the subobject\n";
    external_lib_template_fxn(o,sf_);
    std::cout << "work on object in our Python package:\n";
    of_(o);
}

PYBIND11_PLUGIN(copying4)
{
    py::module m("copying4", "expensive copying");

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
