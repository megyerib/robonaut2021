#include <cstdio>
#include "MazeNavTest.h"
#include "TestCases.h"

// Units under test
#include "MazeNav_AlwaysLeft.h"
#include "mapnavigation.h"

int main()
{
    MazeNav_AlwaysLeft uut;

    size_t passed = 0;

    for (size_t i = 0; i < TEST_CASE_NUM; i++)
    {
        bool success = MazeNavTest::Test(uut, testCases[i]);

        passed += success ? 1 : 0;
        printf("Test case %zu: %s\n", i, success ? "PASS" : "FAIL");
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("%zu/%zu test cases passed (%zu%%)\n\n", passed, TEST_CASE_NUM, passed*100/TEST_CASE_NUM);

    MapNavigation navi;    
    uint16_t      node_count = 48;
    MAZE_MOVE     move;
    MAZE_SECTION  current_section = MAZE_SECTION::sApos;

    // 1
    TURN_INFO turn1Ap_n = { sApos, sAneg,  40, edFrontRight};
    TURN_INFO turn1An_p = { sAneg, sApos,  36, edFrontLeft};
    TURN_INFO turn1Bn_p = { sBneg, sBpos,  13, edRearRight};
    TURN_INFO turn1Pn_p = { sPneg, sPpos,   6, edRearLeft};
    navi.RegisterTurns(turn1Ap_n, turn1An_p, tpRight, tpLeft);
    navi.RegisterTurns(turn1Ap_n, turn1Bn_p, tpRight, tpRight);
    navi.RegisterTurns(turn1Ap_n, turn1Pn_p, tpRight, tpLeft);
    navi.RegisterTurns(turn1An_p, turn1Bn_p, tpLeft,  tpRight);
    navi.RegisterTurns(turn1An_p, turn1Pn_p, tpLeft,  tpLeft);
    navi.RegisterTurns(turn1Bn_p, turn1Pn_p, tpRight, tpLeft);

    // 2
    TURN_INFO turn2Pp_n = { sPpos, sPneg,  6, edFrontMid};
    TURN_INFO turn2Cn_p = { sCneg, sCpos, 12, edRearRight};
    TURN_INFO turn2Dn_p = { sDneg, sDpos, 23, edRearLeft};
    navi.RegisterTurns(turn2Pp_n, turn2Cn_p, tpMiddle, tpRight);
    navi.RegisterTurns(turn2Pp_n, turn2Dn_p, tpMiddle, tpLeft);
    navi.RegisterTurns(turn2Cn_p, turn2Dn_p, tpRight,  tpLeft);

    // 3
    TURN_INFO turn3Bp_n = { sBpos, sBneg, 11, edFrontMid};
    TURN_INFO turn3En_p = { sEneg, sEpos, 30, edRearRight};
    TURN_INFO turn3Qn_p = { sQneg, sQpos,  9, edRearLeft};
    navi.RegisterTurns(turn3Bp_n, turn3En_p, tpMiddle, tpRight);
    navi.RegisterTurns(turn3Bp_n, turn3Qn_p, tpMiddle, tpLeft);
    navi.RegisterTurns(turn3En_p, turn3Qn_p, tpRight,  tpLeft);

    // 4
    TURN_INFO turn4Cp_n = { sCpos, sCneg, 15, edFrontLeft};
    TURN_INFO turn4Qp_n = { sQpos, sQneg, 11, edFrontRight};
    TURN_INFO turn4Rn_p = { sRneg, sRpos,  3, edRearMid};
    navi.RegisterTurns(turn4Cp_n, turn4Qp_n, tpLeft,  tpRight);
    navi.RegisterTurns(turn4Cp_n, turn4Rn_p, tpLeft,  tpMiddle);
    navi.RegisterTurns(turn4Qp_n, turn4Rn_p, tpRight, tpMiddle);

    // 5
    TURN_INFO turn5Dp_n = { sDpos, sDneg, 20, edFrontLeft};
    TURN_INFO turn5Rp_n = { sRpos, sRneg,  9, edFrontRight};
    TURN_INFO turn5Fn_p = { sFneg, sFpos,  9, edRearMid};
    navi.RegisterTurns(turn5Dp_n, turn5Rp_n, tpLeft,  tpRight);
    navi.RegisterTurns(turn5Dp_n, turn5Fn_p, tpLeft,  tpMiddle);
    navi.RegisterTurns(turn5Rp_n, turn5Fn_p, tpRight, tpMiddle);

    // 6
    TURN_INFO turn6Fp_n = { sFpos, sFneg,  6, edFrontMid};
    TURN_INFO turn6Gn_p = { sGneg, sGpos, 10, edRearRight};
    TURN_INFO turn6Sn_p = { sSneg, sSpos, 18, edRearMid};
    TURN_INFO turn6Hn_p = { sHneg, sHpos, 17, edRearLeft};
    navi.RegisterTurns(turn6Fp_n, turn6Gn_p, tpMiddle, tpRight);
    navi.RegisterTurns(turn6Fp_n, turn6Sn_p, tpMiddle, tpMiddle);
    navi.RegisterTurns(turn6Fp_n, turn6Hn_p, tpMiddle, tpLeft);
    navi.RegisterTurns(turn6Gn_p, turn6Sn_p, tpRight,  tpMiddle);
    navi.RegisterTurns(turn6Gn_p, turn6Hn_p, tpRight,  tpLeft);
    navi.RegisterTurns(turn6Sn_p, turn6Hn_p, tpMiddle, tpLeft);

    // 7
    TURN_INFO turn7Ep_n = { sEpos, sEneg, 34, edFrontRight};
    TURN_INFO turn7Gp_n = { sGpos, sGneg, 10, edFrontLeft};
    TURN_INFO turn7Tn_p = { sTneg, sTpos, 12, edRearMid};
    navi.RegisterTurns(turn7Ep_n, turn7Gp_n, tpRight, tpLeft);
    navi.RegisterTurns(turn7Ep_n, turn7Tn_p, tpRight, tpMiddle);
    navi.RegisterTurns(turn7Gp_n, turn7Tn_p, tpLeft,  tpMiddle);

    // 8
    TURN_INFO turn8Sp_n = { sSpos, sSneg, 12, edFrontRight};
    TURN_INFO turn8Hp_n = { sHpos, sHneg, 16, edFrontLeft};
    TURN_INFO turn8Un_p = { sUneg, sUpos, 12, edRearMid};
    navi.RegisterTurns(turn8Sp_n, turn8Hp_n, tpRight, tpLeft);
    navi.RegisterTurns(turn8Sp_n, turn8Un_p, tpRight, tpMiddle);
    navi.RegisterTurns(turn8Hp_n, turn8Un_p, tpLeft,  tpMiddle);

    // 9
    TURN_INFO turn9Tp_n = { sTpos, sTneg, 12, edFrontMid};
    TURN_INFO turn9Jn_p = { sJneg, sJpos, 30, edRearRight};
    TURN_INFO turn9In_p = { sIneg, sIpos, 10, edRearLeft};
    navi.RegisterTurns(turn9Tp_n, turn9Jn_p, tpMiddle, tpRight);
    navi.RegisterTurns(turn9Tp_n, turn9In_p, tpMiddle, tpLeft);
    navi.RegisterTurns(turn9Jn_p, turn9In_p, tpRight,  tpLeft);

    // 10
    TURN_INFO turn10Ip_n = { sIpos, sIneg, 11, edFrontRight};
    TURN_INFO turn10Up_n = { sUpos, sUneg, 19, edFrontLeft};
    TURN_INFO turn10Vn_p = { sVneg, sVpos, 18, edRearRight};
    TURN_INFO turn10Kn_p = { sKneg, sKpos, 23, edRearLeft};
    navi.RegisterTurns(turn10Ip_n, turn10Up_n, tpRight, tpLeft);
    navi.RegisterTurns(turn10Ip_n, turn10Vn_p, tpRight, tpRight);
    navi.RegisterTurns(turn10Ip_n, turn10Kn_p, tpRight, tpLeft);
    navi.RegisterTurns(turn10Up_n, turn10Vn_p, tpLeft,  tpRight);
    navi.RegisterTurns(turn10Up_n, turn10Kn_p, tpLeft,  tpLeft);
    navi.RegisterTurns(turn10Vn_p, turn10Kn_p, tpRight, tpLeft);

    // 11
    TURN_INFO turn11Jp_n = { sJpos, sJneg, 36, edFrontRight};
    TURN_INFO turn11Vp_n = { sVpos, sVneg, 27, edFrontMid};
    TURN_INFO turn11Kp_n = { sKpos, sKneg, 26, edFrontLeft};
    TURN_INFO turn11Nn_p = { sNneg, sNpos, 40, edRearRight};
    TURN_INFO turn11Mn_p = { sMneg, sMpos, 24, edRearMid};
    TURN_INFO turn11Ln_p = { sLneg, sLpos, 20, edRearLeft};
    navi.RegisterTurns(turn11Jp_n, turn11Vp_n, tpRight,  tpMiddle);
    navi.RegisterTurns(turn11Jp_n, turn11Kp_n, tpRight,  tpLeft);
    navi.RegisterTurns(turn11Jp_n, turn11Nn_p, tpRight,  tpRight);
    navi.RegisterTurns(turn11Jp_n, turn11Mn_p, tpRight,  tpMiddle);
    navi.RegisterTurns(turn11Jp_n, turn11Ln_p, tpRight,  tpLeft);
    navi.RegisterTurns(turn11Vp_n, turn11Kp_n, tpMiddle, tpLeft);
    navi.RegisterTurns(turn11Vp_n, turn11Nn_p, tpMiddle, tpRight);
    navi.RegisterTurns(turn11Vp_n, turn11Mn_p, tpMiddle, tpMiddle);
    navi.RegisterTurns(turn11Vp_n, turn11Ln_p, tpMiddle, tpLeft);
    navi.RegisterTurns(turn11Kp_n, turn11Nn_p, tpLeft,   tpRight);
    navi.RegisterTurns(turn11Kp_n, turn11Mn_p, tpLeft,   tpMiddle);
    navi.RegisterTurns(turn11Kp_n, turn11Ln_p, tpLeft,   tpLeft);
    navi.RegisterTurns(turn11Nn_p, turn11Mn_p, tpRight,  tpMiddle);
    navi.RegisterTurns(turn11Nn_p, turn11Ln_p, tpRight,  tpLeft);
    navi.RegisterTurns(turn11Mn_p, turn11Ln_p, tpMiddle, tpLeft);

    // 12
    TURN_INFO turn12Mp_n  = { sMpos,  sMneg,  20, edFrontRight};
    TURN_INFO turn12Lp_n  = { sLpos,  sLneg,  24, edFrontLeft};
    TURN_INFO turn12Stn_p = { sStneg, sStpos, 15, edRearMid};
    navi.RegisterTurns(turn12Mp_n, turn12Lp_n,  tpRight, tpLeft);
    navi.RegisterTurns(turn12Mp_n, turn12Stn_p, tpRight, tpMiddle);
    navi.RegisterTurns(turn12Lp_n, turn12Stn_p, tpLeft,  tpMiddle);

    // 13
    TURN_INFO turn13Stp_n = { sStpos, sStneg, 34, edFrontMid};
    TURN_INFO turn13Exp_n = { sExpos, sExneg, 60, edRearRight};
    TURN_INFO turn13On_p  = { sOneg,  sOpos,  18, edRearLeft};
    navi.RegisterTurns(turn13Stp_n, turn13Exp_n, tpMiddle, tpRight);
    navi.RegisterTurns(turn13Stp_n, turn13On_p,  tpMiddle, tpLeft);
    navi.RegisterTurns(turn13Exp_n, turn13On_p,  tpRight,  tpLeft);

    // 14

    // 15
    TURN_INFO turn15Np_n  = { sNpos,  sNneg,  15, edFrontMid};
    TURN_INFO turn15Exn_p = { sExneg, sExpos, 15, edRearMid};
    navi.RegisterTurns(turn15Np_n, turn15Exn_p, tpMiddle, tpMiddle);

    navi.InitMap(node_count);

    navi.SetCurrentSection(MAZE_SECTION::sStpos);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(13);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(13);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(13);
    current_section = navi.GetCurrentSection();

    move = navi.GetNextMove(8);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(8);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(8);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(8);
    current_section = navi.GetCurrentSection();

    move = navi.GetNextMove(17);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(17);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(17);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(17);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(17);
    current_section = navi.GetCurrentSection();

    move = navi.GetNextMove(0);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(0);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(0);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(0);
    current_section = navi.GetCurrentSection();

    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(23);
    current_section = navi.GetCurrentSection();


    return 0;
}
