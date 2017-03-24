#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <iostream>

using individual = std::pair<std::size_t, std::size_t>;

struct gamete
{
    std::vector<std::size_t> mutations;
    gamete(std::vector<std::size_t> m_) : mutations(std::move(m_)) {}
    gamete(const gamete &g) : mutations(g.mutations)
    {
        std::cout << "gamete got copied\n";
    }
};

struct effect
{
    double s;
    effect(double s_) : s(s_) {}
    effect(const effect &e) : s(e.s) { std::cout << "effect got copied\n"; }
};

struct population
{
    std::vector<individual> ind;
    std::vector<gamete> gametes;
    std::vector<effect> effects;
    population() = default;
    population(const population &p)
        : ind(p.ind), gametes(p.gametes), effects(p.effects)
    {
        std::cout << "population got copied\n";
    }
};

population
make_pop()
{
    population p;
    p.effects.emplace_back(-0.2);
    p.effects.emplace_back(0.4);
    p.gametes.emplace_back(std::vector<std::size_t>{0});
    p.gametes.emplace_back(std::vector<std::size_t>{0,1});
    p.ind.resize(10, { 0, 0 });
    p.ind[3].second=1;
    p.ind[5].first=1;
    return p;
}

using individual_fitness_fxn
    = std::function < double(const individual &, const std::vector<gamete> &,
                             const std::vector<effect> &)>;
using population_recorder_function = std::function<void(const population &)>;

void
process_pop(const population &pop, individual_fitness_fxn fitness,
            population_recorder_function recorder)
{
    for (auto &&i : pop.ind)
        {
            //This will result in gametes + 
            //effects getting copy-constructed
            //and passed to fitness.  This 
            //gets extremely expensive in production
            //code.
            std::cout << "calling fitness fxn:\n";
            fitness(i, pop.gametes, pop.effects);
            std::cout << "done\n";
        }
    //no additional copy is made here.
    std::cout << "calling recorder\n";
    recorder(pop);
    std::cout << "done\n";
}

namespace py = pybind11;

PYBIND11_PLUGIN(copying)
{
    py::module m("copying", "expensive copying");

    py::class_<individual>(m, "Individual")
        .def_readonly("first",&individual::first)
        .def_readonly("second",&individual::second);
    py::class_<gamete>(m, "Gamete")
        .def_readonly("mutations",&gamete::mutations);
    py::class_<effect>(m, "Effect")
        .def_readonly("s",&effect::s);
    py::class_<population>(m, "Population")
        .def_readonly("ind",&population::ind)
        .def_readonly("gametes",&population::gametes)

        .def_readonly("effects",&population::effects);

    m.def("make_pop", &make_pop);
    m.def("process_pop",&process_pop);
    return m.ptr();
}
