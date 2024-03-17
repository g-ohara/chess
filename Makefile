# target
TARGET 	= 	chess

# directories
LIBDIR 	=	GenjiChess
OBJDIR	=	$(LIBDIR)/obj

# main files
SRCS	= 	*.cc
DEPEND	=	dependencies.mk

# library files
LIBSRCS	=	$(LIBDIR)/*.cc
LIBHEAD	=	$(LIBDIR)/*.h
LIBOBJS	= 	$(OBJDIR)/*.o
LIBTRG	=	$(OBJDIR)/lib.a
LIBPCH 	= 	$(LIBDIR)/pch.h
LIBGCH  =	$(LIBPCH).gch

# commands
CC		= 	g++
CCFLAGS	=	-O3 -Wall -std=c++14


# chess -> *.cpp(main), lib.a
$(TARGET) :	$(SRCS) $(LIBTRG)
	$(CC) $(CCFLAGS) $^ -o $@ -I $(LIBDIR)

# lib.a -> *.o(GenjiChess)
$(LIBTRG) :	$(LIBOBJS)
	ar rcs $@ $^

# pch.hpp.gch -> pch.hpp
$(LIBGCH) : $(LIBPCH)
	$(CC) $(CCFLAGS) -x c++-header -o $@ $^

$(OBJDIR)/%.o :	$(LIBDIR)/%.cpp
	$(CC) -c $^ -o $@
# 	$(CC) -MM $< -MT $@ >> $(DEPEND)

# include $(DEPEND)

all : $(LIBGCH)
	$(CC) $(CCFLAGS) -c $(LIBSRCS) -include $(LIBPCH)
	mv *.o $(OBJDIR)
