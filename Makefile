# target
TARGET 	= chess

# directories
LIBDIR 	= GenjiChess
OBJDIR  = $(LIBDIR)/obj

# main files
SRCS    = *.cc
DEPEND  = dependencies.mk

# library files
LIBSRCS = $(LIBDIR)/Address.cc $(LIBDIR)/BitBoard.cc $(LIBDIR)/BitBoard.cc \
		  $(LIBDIR)/BitBoardFamily.cc $(LIBDIR)/BitBoardFamilyArray.cc \
		  $(LIBDIR)/Board.cc $(LIBDIR)/LegalMove.cc $(LIBDIR)/Marking.cc \
		  $(LIBDIR)/Move.cc $(LIBDIR)/OpeningList.cc $(LIBDIR)/Piece.cc \
		  $(LIBDIR)/PieceMap.cc $(LIBDIR)/Record.cc $(LIBDIR)/RecordList.cc \
		  $(LIBDIR)/Thinker.cc $(LIBDIR)/Valuation.cc
LIBHEAD = $(LIBDIR)/*.h
LIBOBJS = $(LIBSRCS:.cc=.o)
LIBTRG  = $(OBJDIR)/lib.a
LIBPCH  = $(LIBDIR)/pch.h
LIBGCH  = $(LIBPCH).gch

# commands
CXX         = g++
CXXFLAGS    = -O3 -Wall -std=c++14

all: $(TARGET)

$(TARGET) : $(SRCS) $(LIBTRG)
	$(CXX) $(CXXFLAGS) $^ -o $@ -I $(LIBDIR)

$(LIBTRG) : $(LIBOBJS)
	ar rcs $@ $^

$(LIBGCH) : $(LIBPCH)
	$(CXX) $(CXXFLAGS) -x c++-header -o $@ $^
