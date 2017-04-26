exList = katsura5 katsura6 katsura7 katsura8 schrans−troost \
	 f633 cyclic6 cyclic7 cyclic8 cyclic9 cyclic10
shortList = katsura5 katsura6 katsura7 katsura8 schrans−troost \
	    f633 cyclic6 cyclic7 cyclic8
longList = cyclic7 cyclic8 #cyclic9 cyclic10
modeList = POT TOP g1 g2
optflags = -O3 -loops

fast :
	@for i in $(shortList);\
	do \
		echo "Compiling : 4 x $${i} ...";\
		g++ -o "run$${i}-POT" -D BRIEF $(optflags) "$${i}-POT.cpp" &\
		g++ -o "run$${i}-TOP" -D BRIEF $(optflags) "$${i}-TOP.cpp" &\
		g++ -o "run$${i}-g1" -D BRIEF $(optflags) "$${i}-g1.cpp" &\
		g++ -o "run$${i}-g2" -D BRIEF $(optflags) "$${i}-g2.cpp" &\
		wait;\
	done

sturmfels : sturmfels.cpp gvw.h
	g++ -o sturmfels $(optflags) sturmfels.cpp

bigFast :
	@for i in $(longList);\
	do \
		echo "Compiling : 2 x $${i} ...";\
		g++ -o "run$${i}-POT" -D BRIEF $(optflags) "$${i}-POT.cpp" &\
		g++ -o "run$${i}-g2" -D BRIEF $(optflags) "$${i}-g2.cpp" &\
		wait;\
	done

clean :
	@for i in $(exList) ;\
	do \
		echo "Deleting $${i} ...";\
		rm "run$${i}-POT";\
		rm "run$${i}-TOP";\
		rm "run$${i}-g1";\
		rm "run$${i}-g2";\
	done
	rm *~ a.out

all : fast
	@echo "Big List" > output.txt
	@for i in $(shortList) ;\
		do \
		echo "Running : 4 x $${i} ..." ;\
		echo "−−−−−−−−−−$${i}" >> output.txt ;\
		/usr/bin/time -o output.txt -a --format=",%U,%M,%S)" \
		"./ run$${i}-POT" >> output.txt ;\
		/usr/bin/time -o output.txt -a --format=",%U,%M,%S)" \
		"./ run$${i}-TOP" >> output.txt ;\
		/usr/bin/time -o output.txt -a --format=",%U,%M,%S)" \
		"./ run$${i}-g1" >> output.txt ;\
		/usr/bin/time -o output.txt -a --format=",%U,%M,%S)" \
		"./ run$${i}-g2" >> output.txt ;\
	done

continue : #bigFast
	# @echo ”Running : 2 x cyclic7 ...”
	# @echo ”−−−−−−−−−−−−cyclic7 ” >> output . txt
	# @/usr/bin/time −o output . txt −a −−format=”,%U,%M,%S)” \
	# ./ runcyclic7−POT >> output . txt
	# @/usr/bin/time −o output . txt −a −−format=”,%U,%M,%S)” \
	# ./ runcyclic7−g2 >> output . txt
	@echo "Running : 2 x cyclic8 ..."
	@echo "−−−−−−−−−−−−cyclic8 " >> output.txt
	@/usr/bin/time −o output.txt −a −−format=",%U,%M,%S)" \
	./ runcyclic8−POT >> output.txt
	@/usr/bin/time −o output.txt −a −−format=",%U,%M,%S)" \
	./ runcyclic8−g2 >> output.txt
	# @echo ”Running : 1 x cyclic9 ...”
	# @echo ”−−−−−−−−−−−−cyclic9 ” >> output . txt
	# @/usr/bin/time −o output . txt −a −−format=”,%U,%M,%S)” \
	# ./ runcyclic9−g2 >> output . txt
	# @echo ”Running : 1 x cyclic10 ...”
	# @echo ”−−−−−−−−−−−−cyclic10” >> output . txt
	# @/usr/bin/time −o output . txt −a −−format=”,%U,%M,%S)” \
	# ./ runcyclic10−g2 >> output . txt

cyclic8POT :
	@echo "Running : cyclic8 POT ..."
	@/usr/bin/time -o output.txt -a --format=",%U,%M,%S)" \
	./ runcyclic8-POT >> output.txt

tex :
	g++ -o txt2tex tally / tally.cpp

rounds : all #continue
	@echo ""

pdf :
	@./ txt2tex output.txt output.tex
	@pdflatex output.tex 2> /dev/null > /dev/null
	@evince output.pdf &

prof :
	g++ -pg sturmfels.cpp
	time --portability ./a.out
	@echo ""
	gprof -b > prof.txt
	kprof -f prof.txt -p gprof 2> /dev/null &
	@echo
	@echo
