/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Scanner for the CellML Text format
//==============================================================================

#pragma once

//==============================================================================

#include <QMap>
#include <QString>

//==============================================================================

#include <limits.h>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewScanner
{
public:
    enum TokenType {
        UnknownToken, SingleLineCommentToken, MultilineCommentToken,
        StringToken, IdentifierOrCmetaIdToken, ProperCmetaIdToken, NumberToken,
        InvalidToken,

        // CellML text keywords

        AndToken, AsToken, BetweenToken, CaseToken, CompToken, DefToken,
        EndCompToken, EndDefToken, EndSelToken, ForToken, GroupToken,
        ImportToken, InclToken, MapToken, ModelToken, OtherwiseToken, SelToken,
        UnitToken, UsingToken, VarToken, VarsToken,

        /*AndToken,*/ OrToken, XorToken, NotToken,

        OdeToken,

        AbsToken, CeilToken, ExpToken, FactToken, FloorToken, LnToken, SqrToken,
        SqrtToken,

        SinToken, CosToken, TanToken, SecToken, CscToken, CotToken,
        SinhToken, CoshToken, TanhToken, SechToken, CschToken, CothToken,
        AsinToken, AcosToken, AtanToken, AsecToken, AcscToken, AcotToken,
        AsinhToken, AcoshToken, AtanhToken, AsechToken, AcschToken, AcothToken,

        FirstOneArgumentMathematicalFunctionToken = AbsToken,
        LastOneArgumentMathematicalFunctionToken = AcothToken,

        LogToken,

        FirstOneOrTwoArgumentMathematicalFunctionToken = LogToken,
        LastOneOrTwoArgumentMathematicalFunctionToken = LogToken,

        PowToken, RemToken, RootToken,

        FirstTwoArgumentMathematicalFunctionToken = PowToken,
        LastTwoArgumentMathematicalFunctionToken = RootToken,

        MinToken, MaxToken,

        GcdToken, LcmToken,

        FirstTwoOrMoreArgumentMathematicalFunctionToken = MinToken,
        LastTwoOrMoreArgumentMathematicalFunctionToken = LcmToken,

        TrueToken, FalseToken, NanToken, PiToken, InfToken, EToken,

        FirstMathematicalConstantToken = TrueToken,
        LastMathematicalConstantToken = EToken,

        BaseToken, EncapsulationToken, ContainmentToken,

        AmpereToken, BecquerelToken, CandelaToken, CelsiusToken, CoulombToken,
        DimensionlessToken, FaradToken, GramToken, GrayToken, HenryToken,
        HertzToken, JouleToken, KatalToken, KelvinToken, KilogramToken,
        LiterToken, LitreToken, LumenToken, LuxToken, MeterToken, MetreToken,
        MoleToken, NewtonToken, OhmToken, PascalToken, RadianToken, SecondToken,
        SiemensToken, SievertToken, SteradianToken, TeslaToken, VoltToken,
        WattToken, WeberToken,

        FirstUnitToken = AmpereToken,
        LastUnitToken = WeberToken,

        // CellML text parameter keywords

        PrefToken, ExpoToken, MultToken, OffToken,

        InitToken, PubToken, PrivToken,

        YottaToken, ZettaToken, ExaToken, PetaToken, TeraToken, GigaToken,
        MegaToken, KiloToken, HectoToken, DekaToken, DeciToken, CentiToken,
        MilliToken, MicroToken, NanoToken, PicoToken, FemtoToken, AttoToken,
        ZeptoToken, YoctoToken,

        FirstPrefixToken = YottaToken,
        LastPrefixToken = YoctoToken,

        InToken, OutToken, NoneToken,

        // Miscellaneous

        QuoteToken, CommaToken,
        EqToken, EqEqToken, NeqToken, LtToken, LeqToken, GtToken, GeqToken,
        PlusToken, MinusToken, TimesToken, DivideToken,
        ColonToken, SemiColonToken,
        OpeningBracketToken, ClosingBracketToken,
        OpeningCurlyBracketToken, ClosingCurlyBracketToken,
        EofToken
    };

    typedef QList<TokenType> TokenTypes;

    explicit CellmlTextViewScanner();

    void setText(const QString &pText);

    TokenType tokenType() const;
    int tokenLine() const;
    int tokenColumn() const;
    QString tokenString() const;
    QString tokenComment() const;

    void getNextToken();

private:
    enum CharType {
        OtherChar,
        LetterChar, DigitChar, UnderscoreChar,
        DoubleQuoteChar, QuoteChar, CommaChar,
        EqChar, LtChar, GtChar,
        PlusChar, MinusChar, TimesChar, DivideChar,
        FullStopChar, ColonChar, SemiColonChar,
        OpeningBracketChar, ClosingBracketChar,
        OpeningCurlyBracketChar, ClosingCurlyBracketChar,
        SpaceChar, TabChar, CrChar, LfChar, EofChar
    };

    QString mText;

    const QChar *mChar;

    CharType mCharType;
    int mCharLine;
    int mCharColumn;

    TokenType mTokenType;
    int mTokenLine;
    int mTokenColumn;
    QString mTokenString;
    QString mTokenComment;

    QMap<QString, TokenType> mKeywords;
    QMap<QString, TokenType> mSiUnitKeywords;
    QMap<QString, TokenType> mParameterKeywords;

    bool mWithinParameterBlock;

    void getNextChar();

    void getSingleLineComment();
    void getMultilineComment();
    void getWord();
    void getNumber();
    void getString();
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
