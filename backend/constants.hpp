#ifndef DEFINES
#define DEFINES 
#include<array>
#include<cstdint>
#include "moves.hpp"
#include "Piece.hpp"

struct MagicInfo{
    std::uint64_t mask;
    std::uint64_t magic; 
    std::uint8_t shift;
    std::uint32_t offset;
};


namespace magic{
    constexpr std::array<MagicInfo, 64> rook_magics = {
        {{9115426935197958144ull, 5764611923777880706ull, 52, 0}, {4485655873561051136ull, 36046394582632964ull, 53, 4096}, 
            {6782456361169985536ull, 1729945211226296322ull, 53, 6144}, {7930856604974452736ull, 4612249295335723074ull, 53, 8192}, 
            {8505056726876686336ull, 720585149057602817ull, 53, 10240}, {8792156787827803136ull, 2379590184338460706ull, 53, 12288}, 
            {8935706818303361536ull, 1152948310571680129ull, 53, 14336}, {9079539427579068672ull, 4612829788090991106ull, 52, 16384},
            {35607136465616896ull, 288265917073228288ull, 53, 20480}, {17522093256097792ull, 288793343966904832ull, 54, 22528}, 
            {26493970160820224ull, 985729356923008ull, 54, 23552}, {30979908613181440ull, 11601285838675148928ull, 54, 24576}, 
            {33222877839362048ull, 1460153589694592ull, 54, 25600}, {34344362452452352ull, 689763363996041344ull, 54, 26624}, 
            {34905104758997504ull, 13844135625713787648ull, 54, 27648}, {35466950888980736ull, 41782524051968ull, 53, 28672}, 
            {36167887395782656ull, 599253168816132ull, 53, 30720}, {18082844186263552ull, 24772003685990408ull, 54, 32768}, 
            {9110691325681664ull, 27305271831330944ull, 54, 33792}, {4624614895390720ull, 2450521182139973649ull, 54, 34816}, 
            {2381576680245248ull, 17596491563016ull, 54, 35840}, {1260057572672512ull, 2815791301001248ull, 54, 36864}, 
            {699298018886144ull, 4506075139424260ull, 54, 37888}, {420017753620736ull, 211108516691969ull, 53, 38912}, 
            {36170077829103616ull, 2305843853275439364ull, 53, 40960}, {18085034619584512ull, 282882368790856192ull, 54, 43008}, 
            {9042787892731904ull, 4644354303984128ull, 54, 44032}, {4521664529305600ull, 1024747009738752ull, 54, 45056}, 
            {2261102847592448ull, 141905727854592ull, 54, 46080}, {1130822006735872ull, 153122971991412736ull, 54, 47104}, 
            {565681586307584ull, 4512533197094976ull, 54, 48128}, {283115671060736ull, 290904392742215745ull, 53, 49152}, 
            {36170086385483776ull, 9331458711379771521ull, 53, 51200}, {18085043175964672ull, 2305916212641531905ull, 54, 53248}, 
            {9042522644946944ull, 1125951463760048ull, 54, 54272}, {4521262379438080ull, 5066592532582420ull, 54, 55296}, 
            {2260632246683648ull, 144150411103700224ull, 54, 56320}, {1130317180306432ull, 1146803514769472ull, 54, 57344}, 
            {565159647117824ull, 1155630703409367172ull, 54, 58368}, {282580897300736ull, 140877075398656ull, 53, 59392}, 
            {36170086418907136ull, 9018194379672771ull, 53, 61440}, {18085043209388032ull, 153126786048262704ull, 54, 63488}, 
            {9042521608822784ull, 5217184972276736ull, 54, 64512}, {4521260808540160ull, 141287311284224ull, 54, 65536}, 
            {2260630408398848ull, 292743871652364320ull, 54, 66560}, {1130315208328192ull, 71468792741936ull, 54, 67584}, 
            {565157608292864ull, 684622185297354754ull, 54, 68608}, {282578808340736ull, 450682669408239624ull, 53, 69632}, 
            {36170086419037696ull, 281476327342082ull, 53, 71680}, {18085043209518592ull, 1125968643630628ull, 54, 73728}, 
            {9042521604775424ull, 563158326575112ull, 54, 74752}, {4521260802403840ull, 288793480891768848ull, 54, 75776}, 
            {2260630401218048ull, 2307531997589801474ull, 54, 76800}, {1130315200625152ull, 7318419221258624ull, 54, 77824}, 
            {565157600328704ull, 70404446461952ull, 54, 78848}, {282578800180736ull, 140739908469792ull, 53, 79872}, 
            {36170086419038334ull, 2377900753576050826ull, 52, 81920}, {18085043209519166ull, 36065080936235392ull, 53, 86016}, 
            {9042521604759646ull, 4683752692042966016ull, 53, 88064}, {4521260802379886ull, 144117387367817448ull, 53, 90112}, 
            {2260630401190006ull, 36033745089857537ull, 53, 92160}, {1130315200595066ull, 1297047826345263153ull, 53, 94208}, 
            {565157600297596ull, 198163073977032704ull, 53, 96256}, {282578800148862ull, 4647716465787691282ull, 52, 98304}}
    };  
    constexpr std::array<MagicInfo, 64> bishop_magics = {
        {{18049651735527936ull, 306262418395267600ull, 58, 0}, {9024825867763712ull, 16142097917296019969ull, 59, 64}, 
            {22526811443298304ull, 218430491874361857ull, 59, 96}, {11333774449049600ull, 9799841602700183040ull, 59, 128}, 
            {5667157807464448ull, 282574492664320ull, 59, 160}, {2832480465846272ull, 5837088472007193604ull, 59, 192}, 
            {1134765260406784ull, 10432589782362114048ull, 59, 224}, {567382630219776ull, 3607454503760109576ull, 58, 256}, 
            {70506452091904ull, 12398431803105615872ull, 59, 320}, {35253226045952ull, 4522368643109378ull, 59, 352}, 
            {87995357200384ull, 580550998556944ull, 59, 384}, {44272556441600ull, 72057737420996608ull, 59, 416}, 
            {22137335185408ull, 8873570336768ull, 59, 448}, {11064376819712ull, 11531469328511926272ull, 59, 480}, 
            {4432676798464ull, 2346449090337834048ull, 59, 512}, {2216338399232ull, 599302691098624ull, 59, 544}, 
            {18014673925310464ull, 11313992100913218ull, 59, 576}, {9007336962655232ull, 4504733507127584ull, 59, 608}, 
            {22518341868716544ull, 586048708519412224ull, 57, 640}, {11259172008099840ull, 3458773314237893632ull, 57, 768}, 
            {5629586008178688ull, 19142502305240064ull, 57, 896}, {2814792987328512ull, 18579550371840512ull, 57, 1024}, 
            {1125917221986304ull, 5648243040322058ull, 59, 1152}, {562958610993152ull, 4918532363926923409ull, 59, 1184}, 
            {9077569074761728ull, 14988544709402575872ull, 59, 1216}, {4538784537380864ull, 5633898654730496ull, 59, 1248}, 
            {2339762086609920ull, 4620695418852886529ull, 57, 1280}, {19184279556981248ull, 563516891332641ull, 55, 1408}, 
            {9592139778506752ull, 46480756702707840ull, 55, 1920}, {4514594912477184ull, 2451086605463851040ull, 57, 2432}, 
            {2256197927833600ull, 2306036591984116736ull, 59, 2560}, {1128098963916800ull, 2353166024168260097ull, 59, 2592}, 
            {4539058881568768ull, 71536979739136ull, 59, 2624}, {2269529440784384ull, 282714678798339ull, 59, 2656}, 
            {1135039602493440ull, 9233510635758682624ull, 57, 2688}, {637888545440768ull, 9078392699813952ull, 55, 2816}, 
            {18051867805491712ull, 108649479388823584ull, 55, 3328}, {9024834391117824ull, 1235113302420374656ull, 57, 3840}, 
            {4512412900526080ull, 1157433970187899920ull, 59, 3968}, {2256206450263040ull, 1487331403554097152ull, 59, 4000}, 
            {2269530512441344ull, 288793330421138464ull, 59, 4032}, {1134765256220672ull, 234363381790871552ull, 59, 4064}, 
            {567383701868544ull, 163818437524587520ull, 57, 4096}, {2491752130560ull, 4522609187229696ull, 57, 4224}, 
            {70515108615168ull, 650771247847718912ull, 57, 4352}, {18049651768822272ull, 5192650542161661984ull, 57, 4480}, 
            {9024825867633664ull, 22553869770883136ull, 59, 4608}, {4512412933816832ull, 18014433020743808ull, 59, 4640}, 
            {1134765260406784ull, 2308666834318073896ull, 59, 4672}, {567382630203392ull, 5188430457684886818ull, 59, 4704}, 
            {2216342585344ull, 2450100103143494148ull, 59, 4736}, {9733406720ull, 9894553529887753224ull, 59, 4768}, 
            {275449643008ull, 10953635141099979784ull, 59, 4800}, {70506452221952ull, 9368621996429049856ull, 59, 4832}, 
            {18049651735527424ull, 576465361374937602ull, 59, 4864}, {9024825867763712ull, 72348148777357840ull, 59, 4896}, 
            {567382630219776ull, 2306408159283269636ull, 58, 4928}, {2216338399232ull, 848827809006080ull, 59, 4992}, 
            {8657588224ull, 1199704893292800ull, 59, 5024}, {38021120ull, 577604691075268608ull, 59, 5056}, 
            {1075975168ull, 583251903589130280ull, 59, 5088}, {275415828992ull, 13983698835519709700ull, 59, 5120}, 
            {70506452091904ull, 565166224474624ull, 59, 5152}, {18049651735527936ull, 234189431337255952ull, 58, 5184}}
    };
    constexpr std::array<std::uint64_t, 8> knight_direction_valid = {
        18229723555195321344u,
        71209857637481724u,
        280371153272574u, 
        140185576636287u,
        17802464409370431u,
        4557430888798830336u,
        9187201950435704832u, 
        18374403900871409664u
    };

    
    template<Piece p>
    constexpr std::size_t attack_table_size(){
        static_assert(p == Piece::ROOK | p == Piece::BISHOP | p == Piece::KNIGHT);
        if constexpr(p == Piece::ROOK){
            return 102400;
        }
        else if constexpr(p == Piece::BISHOP){
            return 5248;
        } 
        else{
            return 64;
        }
    }

    constexpr std::uint32_t get_index(std::uint64_t bitboard, const MagicInfo& magic_info){
        return magic_info.offset + ((bitboard * magic_info.magic) >> magic_info.shift);
    }

    template<Piece p> 
    constexpr std::array<std::uint64_t, attack_table_size<p>()> populate_table(){
        std::array<std::uint64_t, attack_table_size<p>()> table{};
        constexpr auto dirs = moves::get_directions<p>();
        constexpr auto dir1 = dirs[0], dir2 = dirs[1], dir3 = dirs[2], dir4 = dirs[3];
        int i = 0;
        for(std::uint64_t pos = 1; pos; pos <<= 1, i++){
            const MagicInfo& magic_info = (p == Piece::ROOK) ? rook_magics[i] : bishop_magics[i]; 
            for(std::uint64_t u = moves::internal_move<dir1>(pos); ; u = moves::internal_move<dir1>(u)){
                for(std::uint64_t r = moves::internal_move<dir2>(pos); ; r = moves::internal_move<dir2>(r)){
                    for(std::uint64_t d = moves::internal_move<dir3>(pos); ; d = moves::internal_move<dir3>(d)){
                        for(std::uint64_t l = moves::internal_move<dir4>(pos); ; l = moves::internal_move<dir4>(l)){
                            std::uint64_t blockers = u | r | d | l;
                            std::uint64_t all_behind_pieces = 
                                moves::internal_ray<dir1>(u) | 
                                moves::internal_ray<dir2>(r) | 
                                moves::internal_ray<dir3>(d) | 
                                moves::internal_ray<dir4>(l);
                            std::uint64_t attack = moves::get_attack<p>(pos, blockers);
                            for(std::uint64_t cur_behind = all_behind_pieces; ; cur_behind = (cur_behind - 1) & all_behind_pieces){
                                std::uint64_t cur_board = cur_behind | blockers;
                                table[get_index(cur_board, magic_info)] = attack;
                                if(!cur_behind) break;
                            }
                            if(!l) break;
                        }
                        if(!d) break;
                    }
                    if(!r) break;
                }
                if(!u) break;
            }
        }
        return table;
    }

    template<>
    constexpr std::array<std::uint64_t, attack_table_size<Piece::KNIGHT>()> populate_table<Piece::KNIGHT>(){
        std::array<std::uint64_t, 64> table;
        constexpr std::array<moves::Direction, 8> shifts = 
        {moves::K1, moves::K2, moves::K3, moves::K4, moves::K5, moves::K6, moves::K7, moves::K8};
        int i = 0;
        for(std::uint64_t pos = 1; pos; pos <<= 1, i++){
            std::uint64_t attack = 0;
            for(int j = 0; j < 8; j++){
                if(pos & knight_direction_valid[j]) attack |= shifts[j];
            }
            table[i] = attack;
        }
        return table;
    }

    constexpr std::array<std::uint64_t, attack_table_size<Piece::ROOK>()> rook_attacks = populate_table<Piece::ROOK>();
    constexpr std::array<std::uint64_t, attack_table_size<Piece::BISHOP>()> bishop_attacks = populate_table<Piece::BISHOP>();
    constexpr std::array<std::uint64_t, attack_table_size<Piece::KNIGHT>()> knight_attacks = populate_table<Piece::KNIGHT>();
}

//Indices for each piece in white/black attributes in the Board class
#define PAWN_INDEX 0
#define ROOK_INDEX 1
#define KNIGHT_INDEX 2  
#define BISHOP_INDEX 3
#define QUEEN_INDEX 4  
#define KING_INDEX 5  

//Bitmasks for whether the given knight move can be made from each position
#define KNIGHT_1_VALID 18229723555195321344ULL //2 right, 1 down
#define KNIGHT_2_VALID 71209857637481724ULL //2 right, 1 up
#define KNIGHT_3_VALID 280371153272574ULL //2 up, 1 right
#define KNIGHT_4_VALID 140185576636287ULL //2 up, 1 left
#define KNIGHT_5_VALID 17802464409370431ULL //2 left, 1 up
#define KNIGHT_6_VALID 4557430888798830336ULL //2 left, 1 down
#define KNIGHT_7_VALID 9187201950435704832ULL //2 down, 1 left
#define KNIGHT_8_VALID 18374403900871409664ULL //2 down, 1 right
  


//Sets the initial positions for each piece type
#define INIT_BLACK_ROOKS 9295429630892703744ULL
#define INIT_BLACK_KNIGHTS 4755801206503243776ULL
#define INIT_BLACK_BISHOPS 2594073385365405696ULL
#define INIT_BLACK_QUEEN 1152921504606846976ULL
#define INIT_BLACK_KING 576460752303423488ULL
#define INIT_BLACK_PAWNS 71776119061217280ULL
#define INIT_WHITE_PAWNS 65280
#define INIT_WHITE_KING 8
#define INIT_WHITE_QUEEN 16
#define INIT_WHITE_BISHOPS 36
#define INIT_WHITE_KNIGHTS 66
#define INIT_WHITE_ROOKS 129

//Top left position of the bitboard (only most significant bit toggled on)
#define BITBOARD_TOP_LEFT 9223372036854775808ULL

//Bitmasks for multiple positions on the board
#define EDGE_OF_BOARD 36822278289781621759ULL //right, left, top, bottom edges bitwised ORed
#define BITBOARD_RIGHT_EDGE 72340172838076673ULL
#define BITBOARD_LEFT_EDGE 9259542123273814144ULL
#define BITBOARD_TOP_EDGE 18374686479671623680ULL
#define BITBOARD_BOTTOM_EDGE 255



#define WHITE_PAWN_ROW 65280
#define BLACK_PAWN_ROW 71776119061217280ULL

//Codes for method returns
#define EMPTY_TILE -1
#define INVALID_TILE 6
#define NO_THREAT_DIR 8

#define TURN 1 
#define CHECK 2  
#define CHECKMATE 4

#endif
