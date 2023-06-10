#include "pch.hpp"
#include "Address.hpp"
#include "Board.hpp"
#include "Valuation.hpp"

void Evaluator::set_coef(const std::vector<int> &arr)
{
    try
    {
        for (int i = 0; i < (int)arr.size(); ++i)
            _coef(i) = arr.at(i);
    }
    catch (std::exception &ex)
    {
        std::cerr << "Exception in Evaluator::set_coef\n";
        throw;
    }
}

Feature Evaluator::get_feature(const StaticBoard &board)
{
    BitBoardFamily bbf = board.getBBF();
    Feature feature;
    int flag = 0;
    for (name_t name = KING; name <= PAWN; ++name)
    {
        BitBoard white_bb = bbf.piece_map(Piece{WHITE, name, true});
        BitBoard black_bb = bbf.piece_map(Piece{BLACK, name, true});

        for (Address a = 0; a.is_valid(); ++a)
        {
            Address white_a = a;
            Address black_a = Address{a.get_file(), 7 - a.get_rank()};
            feature(flag) = static_cast<int>(white_bb.test(white_a));
            feature(flag) -= static_cast<int>(black_bb.test(black_a));
            ++flag;
        }
    }

    int ply = board.getPly();
    if (ply % 2 == 0)
        feature(flag++) = 1;
    else
        feature(flag++) = -1;

    castlingMarker w_cas = board.castlingMark(WHITE);
    castlingMarker b_cas = board.castlingMark(BLACK);
    feature(flag++) = static_cast<int>(w_cas.kingside) -
                      static_cast<int>(b_cas.kingside);
    feature(flag++) = static_cast<int>(w_cas.queenside) -
                      static_cast<int>(b_cas.queenside);

    feature(flag++) = ply;

    return feature;
}

Evaluator::Evaluator(std::string file_name)
{
    std::vector<int> arr;
    std::ifstream ifs(file_name);
    std::string buf;
    while (getline(ifs, buf))
        arr.push_back(stoi(buf));

    set_coef(arr);
    std::cout << "Succeeded reading \"" << file_name << "\"\n";
}

int Evaluator::get_piece_value(Piece piece, Address ad) const
{
    if (piece.exist)
    {
        if (piece.color == BLACK)
            ad.set_rank(7 - ad.get_rank());

        int value = piece_value[(int)piece.name][ad];
        return piece.color == WHITE ? value : -value;
    }
    else
    {
        return 0;
    }
}

int Evaluator::get_castling_value(castlingMarker marker) const
{
    return castling_value[marker.kingside][marker.queenside][marker.done];
}

int Evaluator::board_value(const StaticBoard &board)
{
    try
    {
        return _coef.dot(get_feature(board));
    }
    catch (std::exception &)
    {
        std::cerr << "Exception in Evaluator::board_balue\n";
        throw;
    }
    // int value = 0;
    // for (Address ad = 0; ad.valid(); ++ad)
    // {
    //     Piece piece = board.getPiece(ad);
    //     if (piece.exist)
    //         value += piece_value[(int)piece.name][ad] * (piece.color == WHITE ? 1 : -1);
    // }

    // return value;
}

Evaluator::Evaluator(void)
{
    // _coef = new Eigen::MatrixXi(1, 1);
    for (int piece = KING; piece <= PAWN; ++piece)
    {
        for (Address ad = 0; ad.is_valid(); ++ad)
        {
            piece_value[piece][ad] =
                existance_value[piece] + development_value[piece] * ad.get_rank();

            if ((ad.get_file() == 3 || ad.get_file() == 4) &&
                (ad.get_rank() == 3 || ad.get_rank() == 4))
            {
                piece_value[piece][ad] += centered_pawn_value;
            }
        }
    }

    int value = 0;
    for (int kingside = 0; kingside <= 1; ++kingside)
    {
        for (int queenside = 0; queenside <= 1; ++queenside)
        {
            for (int done = 0; done <= 1; ++done)
            {
                if (done)
                    value = castling_const[3];
                else
                    value = castling_const[kingside + queenside];

                castling_value[kingside][queenside][done] = value;
            }
        }
    }
}

Evaluator::~Evaluator(void)
{
}
