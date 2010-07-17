#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# square --
# This script generates a demo application containing only a "square"
# widget.  It's only usable in the "tktest" application or if Tk has
# been compiled with tkSquare.c. This demo arranges the following
# bindings for the widget:
# 
# Button-1 press/drag:		moves square to mouse
# "a":				toggle size animation on/off
#
# RCS: @(#) $Id: square,v 1.2 1998/09/14 18:23:30 stanton Exp $

square .s
pack .s -expand yes -fill both
wm minsize . 1 1

bind .s <1> {center %x %y}
bind .s <B1-Motion> {center %x %y}
bind .s a animate
focus .s

# The procedure below centers the square on a given position.

proc center {x y} {
    set a [.s size]
    .s position [expr $x-($a/2)] [expr $y-($a/2)]
}

# The procedures below provide a simple form of animation where
# the box changes size in a pulsing pattern: larger, smaller, larger,
# and so on.

set inc 0
proc animate {} {
    global inc
    if {$inc == 0} {
	set inc 3
	timer
    } else {
	set inc 0
    }
}

proc timer {} {
    global inc
    set s [.s size]
    if {$inc == 0} return
    if {$s >= 40} {set inc -3}
    if {$s <= 10} {set inc 3}
    .s size [expr {$s+$inc}]
    after 30 timer
}
