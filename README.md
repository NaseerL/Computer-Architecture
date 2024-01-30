# Computer-Architecture

The designed system(Event-Driven-Simulator_ 4-Stage-Pipeline) is a single-threaded, event-driven simulator for a 4-stage pipelined CPU executing a RISC-V assembly "for loop." It tracks instruction progress through Fetch, Decode, Execute, and Store stages, printing relevant information at each stage and maintaining an Event List. The simulation continues until the loop completes, handling predictive fetches and discarding instructions before executing a halt instruction.
