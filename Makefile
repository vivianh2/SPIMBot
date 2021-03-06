.PHONY: clean how c_path_planning parse_map parse_weight
.PHONY: test_path_planning test_solve_puzzle
.PHONY: alpha_bot alpha_navigator
.PHONY: turn_right_bot

# Example to create new file
test_solve_puzzle: TARGET_MAIN_FILE = test_solve_puzzle.s
test_solve_puzzle: qtspimbot_run

test_path_planning: TARGET_MAIN_FILE = test_pp.s
test_path_planning: qtspimbot_run

c_path_planning: OUTPUT_EXE = c_path_planning
c_path_planning: SRC_FILES = C/*.c
c_path_planning: c_compile_and_run

alpha_bot: TARGET_MAIN_FILE = alpha_bot.s
alpha_bot: qtspimbot_run

alpha_navigator: TARGET_MAIN_FILE = alpha_navigator.s
alpha_navigator: qtspimbot_run

beta_navigator: TARGET_MAIN_FILE = beta_navigator.s
beta_navigator: qtspimbot_run

astar_bot: TARGET_MAIN_FILE = astar_bot.s
astar_bot: qtspimbot_run

final_bot: TARGET_MAIN_FILE = final.s
final_bot: qtspimbot_run

turn_right_bot: TARGET_MAIN_FILE = turn_right.s
turn_right_bot: qtspimbot_run

parse_map: parse_map_run

parse_weight: parse_weight_run

# Rules to clean
clean:
	rm ./spimbot.s
	rm ./c_path_planning

# Rules to display helper
how:
	@echo "Example Usage: make test_solve_puzzle"
	@echo "This will assemble lib files along with main/test_solve_puzzle.s to form a file ready for submission"
	@echo "and perform a test run in QtSpimBot"

# Actual rule to make the targets
qtspimbot_run:
	python3 ./tool/assembler.py ${TARGET_MAIN_FILE}
	QtSpimbot -file spimbot.s -puzzleseed 1221 -mapseed 233 -debug -prof_file profile.txt -maponly -run -exit_when_done -tournament #

parse_map_run:
	python3 ./tool/map_parser.py > ./C/make_map.c

parse_weight_run:
	python3 ./tool/weight_extract.py > ./C/make_weight.c

c_compile_and_run:
	gcc -o ${OUTPUT_EXE} ${SRC_FILES}
	./${OUTPUT_EXE}
