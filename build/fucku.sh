make
ctest -R "fsm_stream_reassembler_dup"
clear
ctest -R "fsm_stream_reassembler_dup" --rerun-failed --output-on-failure