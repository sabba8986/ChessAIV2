#ifndef DEFINES
#define DEFINES 
#include<array>
#include<cstdint>
#include "bitboard.hpp"
#include "piece.hpp"
#include <algorithm>
#include <bit>

struct MagicInfo{
    std::uint64_t mask;
    std::uint64_t magic; 
    std::uint8_t shift;
    std::uint32_t offset;
};

namespace defaults{
    constexpr std::array<std::uint64_t, 7> white_init = {255ull << 8, 129, 66, 36, 16, 8, 0};
    constexpr std::array<std::uint64_t, 7> black_init = [](){
        std::array<std::uint64_t, 7> init;
        std::transform(white_init.begin(), white_init.end(), init.begin(), 
                        [](const auto& board){return std::byteswap(board);});
        return init;
    }();
    
    constexpr std::array<Piece, 64> pieces_init = [](){ 
        std::array<Piece, 64> pieces{};
        int i = 0;
        for(std::uint64_t pos = 1; pos; pos <<= 1, i++){
            bool matched = false;
            for(int j = 0; j < 6; j++){
                if(pos & white_init[j]){
                    matched = true;
                    pieces[i].type = static_cast<PieceType>(j);
                    pieces[i].color = WHITE;
                }
                else if(pos & black_init[j]){
                    matched = true;
                    pieces[i].type = static_cast<PieceType>(j);
                    pieces[i].color = BLACK;
                }
            }
            if(!matched){
                pieces[i].type = EMPTY;

            }
        }
        return pieces;
    }();

    constexpr std::uint64_t all_white_init = [](){
        std::uint64_t pieces = 0;
        for(auto occ: white_init){
            pieces |= occ;
        }
        return pieces;
    }();

    constexpr std::uint64_t all_black_init = [](){
        std::uint64_t pieces = 0;
        for(auto occ: black_init){
            pieces |= occ;
        }
        return pieces;
    }();

    constexpr std::uint8_t castle_rights_init = 15;
}


namespace tables{
    constexpr std::array<MagicInfo, 64> rook_magics = { 
        {{282578800148862ull, 14447547675473690624ull, 52, 0ul}, {565157600297596ull, 306249241446056256ull, 53, 4096ul}, 
            {1130315200595066ull, 1224988723672776784ull, 53, 6144ul}, {2260630401190006ull, 72067627886903300ull, 53, 8192ul}, 
            {4521260802379886ull, 216177214536811088ull, 53, 10240ul}, {9042521604759646ull, 9943965669345329160ull, 53, 12288ul}, 
            {18085043209519166ull, 144117387115988996ull, 53, 14336ul}, {36170086419038334ull, 1261008445489824512ull, 52, 16384ul}, 
            {282578800180736ull, 1448048035941687424ull, 53, 20480ul}, {565157600328704ull, 288300745433808960ull, 54, 22528ul}, 
            {1130315200625152ull, 577445983449817092ull, 54, 23552ul}, {2260630401218048ull, 9224075793016553856ull, 54, 24576ul}, 
            {4521260802403840ull, 434315957918302214ull, 54, 25600ul}, {9042521604775424ull, 562958548617220ull, 54, 26624ul}, 
            {18085043209518592ull, 5207291372519936ull, 54, 27648ul}, {36170086419037696ull, 2882444503300456576ull, 53, 28672ul}, 
            {282578808340736ull, 44530229329920ull, 53, 30720ul}, {565157608292864ull, 648800921437864580ull, 54, 32768ul}, 
            {1130315208328192ull, 9241423819036475912ull, 54, 33792ul}, {2260630408398848ull, 2306442243319269636ull, 54, 34816ul}, 
            {4521260808540160ull, 1164321791053671424ull, 54, 35840ul}, {9042521608822784ull, 2311332321048921088ull, 54, 36864ul}, 
            {18085043209388032ull, 54082778349774352ull, 54, 37888ul}, {36170086418907136ull, 24189935550785ull, 53, 38912ul}, 
            {282580897300736ull, 9835861939438231556ull, 53, 40960ul}, {565159647117824ull, 288265578777432064ull, 54, 43008ul}, 
            {1130317180306432ull, 2320641340508995650ull, 54, 44032ul}, {2260632246683648ull, 4900206807383803136ull, 54, 45056ul}, 
            {4521262379438080ull, 13835339603274639360ull, 54, 46080ul}, {9042522644946944ull, 585542890166435888ull, 54, 47104ul}, 
            {18085043175964672ull, 216175582433511433ull, 54, 48128ul}, {36170086385483776ull, 1233986581370016004ull, 53, 49152ul}, 
            {283115671060736ull, 1170938240660668544ull, 53, 51200ul}, {565681586307584ull, 36595051983290400ull, 54, 53248ul}, 
            {1130822006735872ull, 1144592695058432ull, 54, 54272ul}, {2261102847592448ull, 144255959932342273ull, 54, 55296ul}, 
            {4521664529305600ull, 1154132651041362176ull, 54, 56320ul}, {9042787892731904ull, 9229002266541819008ull, 54, 57344ul}, 
            {18085034619584512ull, 3379905521790984ull, 54, 58368ul}, {36170077829103616ull, 106120219722345ull, 53, 59392ul}, 
            {420017753620736ull, 600058546651137ull, 53, 61440ul}, {699298018886144ull, 9516176519085441024ull, 54, 63488ul}, 
            {1260057572672512ull, 4521192350318720ull, 54, 64512ul}, {2381576680245248ull, 281612684165160ull, 54, 65536ul}, 
            {4624614895390720ull, 2314859008940376080ull, 54, 66560ul}, {9110691325681664ull, 1688922941947912ull, 54, 67584ul}, 
            {18082844186263552ull, 436866897808719912ull, 54, 68608ul}, {36167887395782656ull, 432417034635444228ull, 53, 69632ul}, 
            {35466950888980736ull, 216736011242340864ull, 53, 71680ul}, {34905104758997504ull, 171141458806710400ull, 54, 73728ul}, 
            {34344362452452352ull, 83334187458756992ull, 54, 74752ul}, {33222877839362048ull, 140806208356992ull, 54, 75776ul}, 
            {30979908613181440ull, 845558810743040ull, 54, 76800ul}, {26493970160820224ull, 116548836556928ull, 54, 77824ul}, 
            {17522093256097792ull, 167196463674692096ull, 54, 78848ul}, {35607136465616896ull, 2310346896629301760ull, 53, 79872ul}, 
            {9079539427579068672ull, 3413438177226818ull, 52, 81920ul}, {8935706818303361536ull, 576497109239464066ull, 53, 86016ul}, 
            {8792156787827803136ull, 153193314439434250ull, 53, 88064ul}, {8505056726876686336ull, 36046389282605313ull, 53, 90112ul}, 
            {7930856604974452736ull, 874261415169888530ull, 53, 92160ul}, {6782456361169985536ull, 4611967519241046037ull, 53, 94208ul}, 
            {4485655873561051136ull, 433190006371140769ull, 53, 96256ul}, {9115426935197958144ull, 2359904347225129990ull, 52, 98304ul}}
    }; 

    constexpr std::array<MagicInfo, 64> bishop_magics = {
        {{18049651735527936ull, 11909918676289568ull, 58, 0}, {70506452091904ull, 9225661324384805888ull, 59, 64}, 
            {275415828992ull, 10379750431860459657ull, 59, 96}, {1075975168ull, 9809970297108562209ull, 59, 128}, 
            {38021120ull, 578791991833464976ull, 59, 160}, {8657588224ull, 19704416735134080ull, 59, 192}, 
            {2216338399232ull, 1193603452339491010ull, 59, 224}, {567382630219776ull, 9300501317271425092ull, 58, 256}, 
            {9024825867763712ull, 493428408451594ull, 59, 320}, {18049651735527424ull, 180152789845150752ull, 59, 352}, 
            {70506452221952ull, 6989591054185938956ull, 59, 384}, {275449643008ull, 4415234768912ull, 59, 416}, 
            {9733406720ull, 1152961108567394304ull, 59, 448}, {2216342585344ull, 160538646807680ull, 59, 480}, 
            {567382630203392ull, 4504705850935424ull, 59, 512}, {1134765260406784ull, 4612249521560224832ull, 59, 544}, 
            {4512412933816832ull, 623185632933971968ull, 59, 576}, {9024825867633664ull, 20266207047410688ull, 59, 608}, 
            {18049651768822272ull, 2308095152631619616ull, 57, 640}, {70515108615168ull, 4613937823081308164ull, 57, 768}, 
            {2491752130560ull, 9583801036668142593ull, 57, 896}, {567383701868544ull, 9223935137146339840ull, 57, 1024}, 
            {1134765256220672ull, 9259964002920898562ull, 59, 1152}, {2269530512441344ull, 2288088009478144ull, 59, 1184}, 
            {2256206450263040ull, 19144748290805776ull, 59, 1216}, {4512412900526080ull, 2832377923569224ull, 59, 1248}, 
            {9024834391117824ull, 18313466259915265ull, 57, 1280}, {18051867805491712ull, 4760314014876303440ull, 55, 1408}, 
            {637888545440768ull, 360570544816668672ull, 55, 1920}, {1135039602493440ull, 10378906530861850632ull, 57, 2432}, 
            {2269529440784384ull, 1225548645678747656ull, 59, 2560}, {4539058881568768ull, 1155358435798221826ull, 59, 2592}, 
            {1128098963916800ull, 108949026653085832ull, 59, 2624}, {2256197927833600ull, 9223946395655874820ull, 59, 2656}, 
            {4514594912477184ull, 4612257919092787200ull, 57, 2688}, {9592139778506752ull, 4611897433897599488ull, 55, 2816}, 
            {19184279556981248ull, 1170937002901700672ull, 55, 3328}, {2339762086609920ull, 1297320512711591938ull, 57, 3840}, 
            {4538784537380864ull, 181274300263923944ull, 59, 3968}, {9077569074761728ull, 19283236006265104ull, 59, 4000},
            {562958610993152ull, 2257333350566404ull, 59, 4032}, {1125917221986304ull, 288802139411629060ull, 59, 4064}, 
            {2814792987328512ull, 288443682485437440ull, 57, 4096}, {5629586008178688ull, 6605693256737ull, 57, 4224}, 
            {11259172008099840ull, 5700007898415392ull, 57, 4352}, {22518341868716544ull, 13844091986417549600ull, 57, 4480}, 
            {9007336962655232ull, 92346895594160644ull, 59, 4608}, {18014673925310464ull, 1147909500320040ull, 59, 4640}, 
            {2216338399232ull, 38584620660359170ull, 59, 4672}, {4432676798464ull, 2450523350713763876ull, 59, 4704}, 
            {11064376819712ull, 72620822160744722ull, 59, 4736}, {22137335185408ull, 36030996588003328ull, 59, 4768},
            {44272556441600ull, 4763695838000349184ull, 59, 4800}, {87995357200384ull, 2454761001657352208ull, 59, 4832}, 
            {35253226045952ull, 4622135923168051217ull, 59, 4864}, {70506452091904ull, 2394739076825092ull, 59, 4896}, 
            {567382630219776ull, 9332022494842077696ull, 58, 4928}, {1134765260406784ull, 9228441064699855624ull, 59, 4992}, 
            {2832480465846272ull, 4764812942518780932ull, 59, 5024}, {5667157807464448ull, 2594086683264943104ull, 59, 5056}, 
            {11333774449049600ull, 282603125167293442ull, 59, 5088}, {22526811443298304ull, 163590477911167488ull, 59, 5120}, 
            {9024825867763712ull, 1411843327345668ull, 59, 5152}, {18049651735527936ull, 1157796766142206208ull, 58, 5184}}
    };


    constexpr std::size_t table_size(PieceType p){
        switch(p){
            case ROOK: return 102400ul;
            case BISHOP: return 5248;
            default: return 64;
        }
    }

    constexpr std::uint32_t get_index_from_magic(std::uint64_t bitboard, const MagicInfo& magic_info){
        return magic_info.offset + ((bitboard * magic_info.magic) >> magic_info.shift);
    }

    template<PieceType p> 
    constexpr std::array<std::uint64_t, table_size(p)> populate_table(){
        using namespace bitboard;
        std::array<std::uint64_t, table_size(p)> table{};
        auto dirs = get_directions(p);
        auto dir1 = dirs[0], dir2 = dirs[1], dir3 = dirs[2], dir4 = dirs[3];
        int i = 0;
        for(std::uint64_t pos = 1; pos; pos <<= 1, i++){
            const MagicInfo& magic_info = (p == ROOK) ? rook_magics[i] : bishop_magics[i]; 
            for(std::uint64_t u = internal_slide(pos, dir1); ; u = internal_slide(u, dir1)){
                for(std::uint64_t r = internal_slide(pos, dir2); ; r = internal_slide(r, dir2)){
                    for(std::uint64_t d = internal_slide(pos, dir3); ; d = internal_slide(d, dir3)){
                        for(std::uint64_t l = internal_slide(pos, dir4); ; l = internal_slide(l, dir4)){
                            std::uint64_t blockers = u | r | d | l;
                            std::uint64_t all_behind_pieces = 
                                internal_ray(u, dir1) | 
                                internal_ray(r, dir2) | 
                                internal_ray(d, dir3) | 
                                internal_ray(l, dir4);
                            std::uint64_t attack = raycast_attack(pos, p, blockers);
                            for(std::uint64_t cur_behind = all_behind_pieces; ; cur_behind = (cur_behind - 1) & all_behind_pieces){
                                std::uint64_t cur_board = cur_behind | blockers;
                                table[get_index_from_magic(cur_board, magic_info)] = attack;
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
    constexpr std::array<std::uint64_t, table_size(KNIGHT)> populate_table<KNIGHT>(){
        using namespace bitboard;
        std::array<std::uint64_t, 8> direction_valid = {
            18229723555195321344ull,
            71209857637481724ull,
            280371153272574ull, 
            140185576636287ull,
            17802464409370431ull,
            4557430888798830336ull,
            9187201950435704832ull, 
            18374403900871409664ull
        };
        std::array<std::uint64_t, 64> table{};
        auto dirs = bitboard::get_directions(KNIGHT); 
        int i = 0;
        for(std::uint64_t pos = 1; pos; pos <<= 1, i++){
            for(int j = 0; j < 8; j++){
                if(pos & direction_valid[j]) table[i] |= shift(pos, dirs[j]);
            }
        }
        return table;
    }

    template<>
    constexpr std::array<std::uint64_t, table_size(KING)> populate_table<KING>(){
        using namespace bitboard;
        std::array<std::uint64_t, 64> table{};
        auto dirs = get_directions(KING);
        int i = 0;
        for(std::uint64_t pos = 1; pos; pos <<= 1, i++){ 
            for(int j = 0; j < 8; j++){
                table[i] |= slide(pos, dirs[j]);
            }
        }
        return table;
    }

    constexpr std::array<std::uint64_t, table_size(ROOK)> rook_attacks = populate_table<ROOK>();
    constexpr std::array<std::uint64_t, table_size(BISHOP)> bishop_attacks = populate_table<BISHOP>();
    constexpr std::array<std::uint64_t, table_size(KNIGHT)> knight_attacks = populate_table<KNIGHT>();
    constexpr std::array<std::uint64_t, table_size(KING)> king_attacks = populate_table<KING>();

    constexpr auto attack_from_piece_to_king = [](){
        using namespace bitboard;
        std::array<std::array<std::uint64_t, 64>, 64> table{};
        for(int i = 0; i < 64; i++){
            for(int j = i + 1; j < 64; j++){
                std::uint64_t a = 1ull << i;
                std::uint64_t b = 1ull << j;
                std::uint64_t attack_ray = 
                    (ray<NE>(a) & ray<SW>(b)) | 
                    (ray<N>(a) & ray<S>(b)) | 
                    (ray<NW>(a) & ray<SE>(b)) | 
                    (ray<W>(a) & ray<E>(b)); 
                table[i][j] = attack_ray | a;
                table[j][i] = attack_ray | b;
            }
        }
        return table;
    }();
}

#endif
