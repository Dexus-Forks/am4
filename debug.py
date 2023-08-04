import am4utils
from am4utils.aircraft import Aircraft
from am4utils.airport import Airport
from am4utils.route import AircraftRoute
from am4utils.game import User

import matplotlib.pyplot as plt
# plt.switch_backend('PyQt6')

if __name__ == '__main__':
    print(f'py: am4utils ({am4utils._core.__version__}), executable_path={am4utils.__path__[0]}')
    am4utils.db.init()

    ap0 = Airport.search('VHHH').ap
    ap1 = Airport.search('TPE').ap
    ac = Aircraft.search('mc214').ac
    options = AircraftRoute.Options(tpd_mode=AircraftRoute.Options.TPDMode.AUTO_MULTIPLE_OF, trips_per_day=2)
    # options = AircraftRoute.Options(tpd_mode=AircraftRoute.Options.TPDMode.AUTO)
    user = User.Default()
    user.income_loss_tol = 1
    
    r = AircraftRoute.create(ap0, ap1, ac, options, user)
    # print(r.to_dict())
    print(r.income, r.config, r.trips_per_day)
    # X, Y = [], []
    # for tpd in range(1, 3000):
    #     options = AircraftRoute.Options(tpd_mode=AircraftRoute.Options.TPDMode.STRICT, trips_per_day=tpd)
    #     r = AircraftRoute.create(ap0, ap1, ac, options=options)
    #     if AircraftRoute.Warning.ERR_INSUFFICIENT_DEMAND in r.warnings:
    #         break
    #     print(f'{tpd} {r.max_income}')
    #     print(r)
    #     X.append(tpd)
    #     Y.append(r.max_income)
    # plt.plot(X, [y / max(Y) for y in Y])
    # plt.show()