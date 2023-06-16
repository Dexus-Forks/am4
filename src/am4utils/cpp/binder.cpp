#include <pybind11/pybind11.h>

#include "include/db.hpp"
#include "include/user.hpp"
#include "include/ticket.hpp"
#include "include/demand.hpp"

#include "include/airport.hpp"
#include "include/aircraft.hpp"
#include "include/route.hpp"

#ifdef VERSION_INFO
    string version = MACRO_STRINGIFY(VERSION_INFO);
#else
    string version = "dev";
#endif

namespace py = pybind11;
using namespace pybind11::literals;
using std::string;

PYBIND11_MODULE(_core, m) {
    py::module_ m_db = m.def_submodule("db");
    py::module_ m_user = m.def_submodule("user");
    py::module_ m_ticket = m.def_submodule("ticket");
    py::module_ m_demand = m.def_submodule("demand");
    py::module_ m_ac = m.def_submodule("aircraft");
    py::module_ m_ap = m.def_submodule("airport");
    py::module_ m_route = m.def_submodule("route");

    /*** DATABASE ***/
    m_db
        .def("init", &init)
        .def("_debug_query", &_debug_query);

    py::register_exception<DatabaseException>(m_db, "DatabaseException");

    // needs to be defined before classes for default arguments to work
    py::enum_<User::GameMode>(m_user, "GameMode")
        .value("EASY", User::GameMode::EASY)
        .value("REALISM", User::GameMode::REALISM);

    /*** AIRCRAFT ***/
    py::class_<Aircraft> ac_class(m_ac, "Aircraft");
    ac_class
        .def(py::init())
        .def_readonly("id", &Aircraft::id)
        .def_readonly("shortname", &Aircraft::shortname)
        .def_readonly("manufacturer", &Aircraft::manufacturer)
        .def_readonly("name", &Aircraft::name)
        .def_readonly("type", &Aircraft::type)
        .def_readonly("priority", &Aircraft::priority)
        .def_readonly("eid", &Aircraft::eid)
        .def_readonly("ename", &Aircraft::ename)
        .def_readonly("speed", &Aircraft::speed)
        .def_readonly("fuel", &Aircraft::fuel)
        .def_readonly("co2", &Aircraft::co2)
        .def_readonly("cost", &Aircraft::cost)
        .def_readonly("capacity", &Aircraft::capacity)
        .def_readonly("rwy", &Aircraft::rwy)
        .def_readonly("check_cost", &Aircraft::check_cost)
        .def_readonly("range", &Aircraft::range)
        .def_readonly("ceil", &Aircraft::ceil)
        .def_readonly("maint", &Aircraft::maint)
        .def_readonly("pilots", &Aircraft::pilots)
        .def_readonly("crew", &Aircraft::crew)
        .def_readonly("engineers", &Aircraft::engineers)
        .def_readonly("technicians", &Aircraft::technicians)
        .def_readonly("img", &Aircraft::img)
        .def_readonly("wingspan", &Aircraft::wingspan)
        .def_readonly("length", &Aircraft::length)
        .def_readonly("valid", &Aircraft::valid)
        .def_static("from_str", &Aircraft::from_str, "s"_a)
        .def("__repr__", [](const Aircraft &ac) {
            return Aircraft::repr(ac);
        });
    
    py::enum_<Aircraft::Type>(ac_class, "Type")
        .value("PAX", Aircraft::Type::PAX)
        .value("CARGO", Aircraft::Type::CARGO)
        .value("VIP", Aircraft::Type::VIP);
    
    py::class_<PurchasedAircraft, Aircraft> p_ac_class(m_ac, "PurchasedAircraft");
    p_ac_class
        .def(py::init<>())
        .def_readonly("config", &PurchasedAircraft::config)
        .def("__repr__", [](const PurchasedAircraft &ac) {
            return PurchasedAircraft::repr(ac);
        })
    
    py::class_<PurchasedAircraft::Config>(p_ac_class, "Config")
        .def(py::init<>())
        .def_readonly("pax_config", &PurchasedAircraft::Config::pax_config)
        .def_readonly("cargo_config", &PurchasedAircraft::Config::cargo_config);
    
    py::class_<PaxConfig> pc_class(m_ac, "PaxConfig");
    pc_class
        .def(py::init<>())
        .def_readonly("y", &PaxConfig::y)
        .def_readonly("j", &PaxConfig::j)
        .def_readonly("f", &PaxConfig::f)
        .def_readonly("valid", &PaxConfig::valid)
        .def_readonly("algorithm", &PaxConfig::algorithm);

    py::enum_<PaxConfig::Algorithm>(pc_class, "Algorithm")
        .value("FJY", PaxConfig::Algorithm::FJY).value("FYJ", PaxConfig::Algorithm::FYJ)
        .value("JFY", PaxConfig::Algorithm::JFY).value("JYF", PaxConfig::Algorithm::JYF)
        .value("YJF", PaxConfig::Algorithm::YJF).value("YFJ", PaxConfig::Algorithm::YFJ)
        .value("NONE", PaxConfig::Algorithm::NONE);

    py::class_<CargoConfig> cc_class(m_ac, "CargoConfig");
    cc_class
        .def(py::init<>())
        .def_readonly("l", &CargoConfig::l)
        .def_readonly("h", &CargoConfig::h)
        .def_readonly("valid", &CargoConfig::valid)
        .def_readonly("algorithm", &CargoConfig::algorithm);

    py::enum_<CargoConfig::Algorithm>(cc_class, "Algorithm")
        .value("L", CargoConfig::Algorithm::L).value("H", CargoConfig::Algorithm::H)
        .value("NONE", CargoConfig::Algorithm::NONE);

    py::register_exception<AircraftNotFoundException>(m_ac, "AircraftNotFoundException");


    /*** AIRPORT ***/
    py::class_<Airport>(m_ap, "Airport")
        .def(py::init<>())
        .def_readonly("id", &Airport::id)
        .def_readonly("name", &Airport::name)
        .def_readonly("fullname", &Airport::fullname)
        .def_readonly("country", &Airport::country)
        .def_readonly("continent", &Airport::continent)
        .def_readonly("iata", &Airport::iata)
        .def_readonly("icao", &Airport::icao)
        .def_readonly("lat", &Airport::lat)
        .def_readonly("lng", &Airport::lng)
        .def_readonly("rwy", &Airport::rwy)
        .def_readonly("market", &Airport::market)
        .def_readonly("hub_cost", &Airport::hub_cost)
        .def_readonly("rwy_codes", &Airport::rwy_codes)
        .def_readonly("valid", &Airport::valid)
        .def_static("from_str", &Airport::from_str, "s"_a)
        .def("__repr__", [](const Airport &ac) {
            return Airport::repr(ac);
        });

    py::register_exception<AirportNotFoundException>(m_ap, "AirportNotFoundException");


    /*** ROUTE ***/
    py::class_<Route>(m_route, "Route")
        .def(py::init<>())
        .def_readonly("origin", &Route::origin)
        .def_readonly("destination", &Route::destination)
        .def_readonly("pax_demand", &Route::pax_demand)
        .def_readonly("cargo_demand", &Route::cargo_demand)
        .def_readonly("aircraft", &Route::aircraft)
        .def_readonly("ticket", &Route::ticket)
        .def_readonly("income", &Route::income)
        .def_readonly("direct_distance", &Route::direct_distance)
        .def_readonly("valid", &Route::valid)
        .def_static("create_optimal_pax_ticket", &PaxTicket::from_optimal, "distance"_a, "game_mode"_a)
        .def_static("create_optimal_cargo_ticket", &CargoTicket::from_optimal, "distance"_a, "game_mode"_a)
        .def_static("from_airports", &Route::from_airports, "ap1"_a, "ap2"_a)
        .def_static("from_airports_with_aircraft", &Route::from_airports_with_aircraft, "ap1"_a, "ap2"_a, "ac"_a, "trips_per_day"_a = 1, "game_mode"_a = User::GameMode::EASY)
        .def("__repr__", [](const Route& r) {
            return Route::repr(r);
        });
        

    py::class_<PaxTicket>(m_ticket, "PaxTicket")
        .def(py::init<>())
        .def_readonly("y", &PaxTicket::y)
        .def_readonly("j", &PaxTicket::j)
        .def_readonly("f", &PaxTicket::f)
        .def("__repr__", [](const PaxTicket &a) {
            return PaxTicket::repr(a);
        });

    py::class_<CargoTicket>(m_ticket, "CargoTicket")
        .def(py::init<>())
        .def_readonly("l", &CargoTicket::l)
        .def_readonly("h", &CargoTicket::h)
        .def("__repr__", [](const CargoTicket &a) {
            return CargoTicket::repr(a);
        });
    
    py::class_<VIPTicket>(m_ticket, "VIPTicket")
        .def(py::init<>())
        .def_readonly("y", &VIPTicket::y)
        .def_readonly("j", &VIPTicket::j)
        .def_readonly("f", &VIPTicket::f)
        .def("__repr__", [](const VIPTicket &a) {
            return VIPTicket::repr(a);
        });
    
    py::class_<Ticket>(m_ticket, "Ticket")
        .def(py::init<>())
        .def_readonly("pax_ticket", &Ticket::pax_ticket)
        .def_readonly("cargo_ticket", &Ticket::cargo_ticket)
        .def_readonly("vip_ticket", &Ticket::vip_ticket);
    
    py::class_<PaxDemand>(m_demand, "PaxDemand")
        .def(py::init<>())
        .def_readonly("y", &PaxDemand::y)
        .def_readonly("j", &PaxDemand::j)
        .def_readonly("f", &PaxDemand::f)
        .def("__repr__", [](const PaxDemand &a) {
            return PaxDemand::repr(a);
        });
    
    py::class_<CargoDemand>(m_demand, "CargoDemand")
        .def(py::init<>())
        .def_readonly("l", &CargoDemand::l)
        .def_readonly("h", &CargoDemand::h)
        .def("__repr__", [](const CargoDemand &a) {
            return CargoDemand::repr(a);
        });
    
    m.attr("__version__") = version;
}