# -*- mode: gdb-script -*-

set verbose off
set confirm off

break elect
commands
  printf "process : v=%d\n", current_process

  if current_process == p1
    set $nb_exec_p1 = $nb_exec_p1 + 1
  end
  if current_process == p2
    set $nb_exec_p2 = $nb_exec_p2 + 1
  end
  if current_process == p3
    set $nb_exec_p3 = $nb_exec_p3 + 1    
  end
end

set $nb_exec_p1 = 0
set $nb_exec_p2 = 0
set $nb_exec_p3 = 0

target remote:1234

# you can put here as much 'continue' as you want, the remainder of the test should be robust
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue


printf "Process 3 has been elected %d times\n", $nb_exec_p3
printf "Process 2 has been elected %d times\n", $nb_exec_p2
printf "Process 1 has been elected %d times\n", $nb_exec_p1

# Get the minimum number of execution of a process
set $min = $nb_exec_p1
if $nb_exec_p2 < $min
  set $min = $nb_exec_p2
end
if $nb_exec_p3 < $min
  set $min = $nb_exec_p2
end

print $min

set $diff1 = $nb_exec_p1 - $min
set $diff2 = $nb_exec_p2 - $min
set $diff3 = $nb_exec_p3 - $min

set $ok = 1
# multiply as a logical "and"
# one process can, at most, have been elected 1 more time than an other 
set $ok *= ($diff1 <= 1)
set $ok *= ($diff2 <= 1)
set $ok *= ($diff3 <= 1)

if $ok
  printf "test OK\n"
else
    printf "test ERROR\n"
end
quit
