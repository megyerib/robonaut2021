#pragma once

#include "mapnavigation.h"
#include <string>
#include <vector>

static const std::string csvHeader = "Ap, An, Bp, Bn, Cp, Cn, Dp, Dn, Ep, En, Fp, Fn, Gp, Gn, Fp, Fn, Hp, Hn, Ip, In, Jp, Jn, Kp, Kn, Lp, Ln, Mp, Mn, Np, Nn, Op, On, Qp, Qn, Rp, Rn, Sp, Sn, Tp, Tn, Up, Un, Vp, Vn, Stp, Stn, Exp, Exn\n";

class MapGenerator
{
public:
    MapGenerator();

    void GenerateMap(TURN_MATRIX const & matrix, uint8_t const vertex_count);

private:
    std::vector<std::string> csv_string;
    std::string file_name;

    void SerializeTurnMatrix(TURN_MATRIX const & matrix, uint8_t const vertex_count);

    void SerializeMoveInfo(MOVE_INFO const move);

    void CreateCsvExport();

};
