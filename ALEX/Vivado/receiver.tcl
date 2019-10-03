# receiver.tcl: Tcl script for re-creating project 'receiver'
#*****************************************************************************************

# Set the reference directory for source file relative paths (by default the value is script directory path)
set origin_dir [file dirname [info script]]

# Use origin directory path location variable, if specified in the tcl shell
if { [info exists ::origin_dir_loc] } {
  set origin_dir $::origin_dir_loc
}

variable script_file
set script_file "receiver.tcl"

# Help information for this script
proc help {} {
  variable script_file
  puts "\nDescription:"
  puts "Recreate a Vivado project from this script. The created project will be"
  puts "functionally equivalent to the original project for which this script was"
  puts "generated. The script contains commands for creating a project, filesets,"
  puts "runs, adding/importing sources and setting properties on various objects.\n"
  puts "Syntax:"
  puts "$script_file"
  puts "$script_file -tclargs \[--origin_dir <path>\]"
  puts "$script_file -tclargs \[--help\]\n"
  puts "Usage:"
  puts "Name                   Description"
  puts "-------------------------------------------------------------------------"
  puts "\[--origin_dir <path>\]  Determine source file paths wrt this path. Default"
  puts "                       origin_dir path value is \".\", otherwise, the value"
  puts "                       that was set with the \"-paths_relative_to\" switch"
  puts "                       when this script was generated.\n"
  puts "\[--help\]               Print help information for this script"
  puts "-------------------------------------------------------------------------\n"
  exit 0
}

if { $::argc > 0 } {
  for {set i 0} {$i < [llength $::argc]} {incr i} {
    set option [string trim [lindex $::argv $i]]
    switch -regexp -- $option {
      "--origin_dir" { incr i; set origin_dir [lindex $::argv $i] }
      "--help"       { help }
      default {
        if { [regexp {^-} $option] } {
          puts "ERROR: Unknown option '$option' specified, please type '$script_file -tclargs --help' for usage info.\n"
          return 1
        }
      }
    }
  }
}

# Set the directory path for the original project from where this script was exported
set orig_proj_dir "[file normalize "$origin_dir/receiver"]"

# Create project
create_project receiver $origin_dir/receiver

# Set the directory path for the new project
set proj_dir [get_property directory [current_project]]

# Set project properties
set obj [get_projects receiver]
set_property "board_part" "em.avnet.com:zed:part0:1.3" $obj
set_property "default_lib" "xil_defaultlib" $obj
set_property "simulator_language" "Mixed" $obj
set_property "target_language" "VHDL" $obj

# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1
}

# Set 'sources_1' fileset object
set obj [get_filesets sources_1]
set files [list \
 "[file normalize "$origin_dir/src/common/hdl/IED_config.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/FEdetector.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/initialize.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/parity_checker.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/receiver.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/receiver_communication.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/rs_receiver.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/hdl/serial2parallel.vhd"]"\
 "[file normalize "$origin_dir/src/receiver/component.xml"]"\
]
add_files -norecurse -fileset $obj $files

# Set 'sources_1' fileset file properties for remote files
set file "$origin_dir/src/common/hdl/IED_config.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/FEdetector.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/initialize.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/parity_checker.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/receiver.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/receiver_communication.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/rs_receiver.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

set file "$origin_dir/src/receiver/hdl/serial2parallel.vhd"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
set_property "file_type" "VHDL" $file_obj

#set file "component.xml"
#set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
#set_property "file_type" "IP-XACT" $file_obj



# Set 'sources_1' fileset file properties for local files
# None

# Set 'sources_1' fileset properties
set obj [get_filesets sources_1]
set_property "top" "receiver" $obj

# Create 'constrs_1' fileset (if not found)
if {[string equal [get_filesets -quiet constrs_1] ""]} {
  create_fileset -constrset constrs_1
}

# Set 'constrs_1' fileset object
set obj [get_filesets constrs_1]

# Empty (no sources present)

# Set 'constrs_1' fileset properties
set obj [get_filesets constrs_1]

# None


# Create 'synth_1' run (if not found)
#if {[string equal [get_runs -quiet synth_1] ""]} {
  #create_run -name synth_1 -part xc7z020clg484-1 -flow {Vivado Synthesis 2015} -strategy "Vivado Synthesis Defaults" -constrset constrs_1
#} else {
 # set_property strategy "Vivado Synthesis Defaults" [get_runs synth_1]
 # set_property flow "Vivado Synthesis 2015" [get_runs synth_1]
#}
#set obj [get_runs synth_1]

# set the current synth run
#current_run -synthesis [get_runs synth_1]

# Create 'impl_1' run (if not found)
#if {[string equal [get_runs -quiet impl_1] ""]} {
  #create_run -name impl_1 -part xc7z020clg484-1 -flow {Vivado Implementation 2015} -strategy "Vivado Implementation Defaults" -constrset constrs_1 -parent_run synth_1
#} else {
  #set_property strategy "Vivado Implementation Defaults" [get_runs impl_1]
  #set_property flow "Vivado Implementation 2015" [get_runs impl_1]
#}
#set obj [get_runs impl_1]
#set_property "steps.write_bitstream.args.readback_file" "0" $obj
#set_property "steps.write_bitstream.args.verbose" "0" $obj

# set the current impl run
#current_run -implementation [get_runs impl_1]

puts "INFO: Project created:receiver"
