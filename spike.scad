
spike_dia = 4;
spike_sleeve = 25;
wall=1.6;
fuzz = 0.1;

$fn = 60;

tilt_angle = 25;

length=35;
width=82+35;
height=45;
clearance=0.2;
radius = 5;

battery_fence_inset = 82;
battery_fence_height = 8;

spike_inset = radius - 1;

spike_spacing = length+2*wall-2*spike_inset;

lid_height = 10;
sensor_length = length+2*wall-2*radius;
sensor_height = 12;
sensor_width = 15;
sensor_up = 10;

pcap_height = 32;
pcap_length = 22.5;
pcap_width = 12;



module lozenge(l,w,h,r) {
    hull() {
        translate([r,r,0]) cylinder(r=r,h=h);
        translate([l-r,r,0]) cylinder(r=r,h=h);
        translate([l-r,w-r,0]) cylinder(r=r,h=h);
        translate([r,w-r,0]) cylinder(r=r,h=h);
    }
}

module halflozenge(l,w,h,r) {
    hull() {
        cube([l,w-r,h]);
        translate([l-r,w-r,0]) cylinder(r=r,h=h);
        translate([r,w-r,0]) cylinder(r=r,h=h);
    }
}

module clozenge(l,w,h,r) {
    translate([-l/2,-w/2,0]) lozenge(l,w,h,r);
}

module spike_post() {
    //lozenge(post_dia, post_dia, spike_sleeve+10, radius+wall/2);
    cylinder(r=spike_inset-wall/2, h=spike_sleeve+10);
}

module lozenge_box(length,width,height,radius,wall) {
    difference() {
	lozenge(length+2*wall, width+2*wall, height+wall, radius);
	translate([wall,wall,wall]) lozenge(length, width, height+wall+fuzz, radius-wall);
    }
}

module halflozenge_box(length,width,height,radius,wall) {
    difference() {
	halflozenge(length+2*wall, width+2*wall, height+wall, radius);
	translate([wall,wall,wall]) halflozenge(length, width, height+fuzz, radius-wall);
    }
}

module body() {
    lozenge_box(length, width, height, radius, wall);
    translate([wall,wall+battery_fence_inset,wall]) cube([length,wall,battery_fence_height]);
}

module hutch() {
    halflozenge_box(sensor_length,sensor_width,wall+sensor_up+sensor_height,radius,wall);
}

module assembly() {
    difference() {
        union() {
            body();
	    translate([wall+length/2-sensor_length/2-wall,wall+width,wall+sensor_up+sensor_height+wall]) scale([1,1,-1]) hutch();
            translate([spike_inset,spike_inset,0]) spike_post();
            translate([length+2*wall-spike_inset,spike_inset,0]) spike_post();
            translate([spike_inset,wall+width/2,0]) spike_post();
            translate([length+2*wall-spike_inset,wall+width/2,0]) spike_post();
        }
        translate([length+2*wall-spike_inset,spike_inset,0]) cylinder(d=spike_dia, h=spike_sleeve);
        translate([spike_inset,spike_inset,0]) cylinder(d=spike_dia, h=spike_sleeve);
        translate([length+2*wall-spike_inset,wall+width/2,0]) cylinder(d=spike_dia, h=spike_sleeve);
        translate([spike_inset,wall+width/2,0]) cylinder(d=spike_dia, h=spike_sleeve);
	translate([wall+length/2-sensor_length/2,wall+width-fuzz,wall+sensor_up]) cube([sensor_length,wall+2*fuzz,sensor_height]);
	translate([0,width+2*wall+sensor_width-8,5*wall]) rotate([-tilt_angle,0,0]) cube([length+2*wall,sensor_width,1.6]);
	translate([wall+length/2-3,width+2*wall+sensor_width-4,-1]) cube([6,6,8]);
    }
}

module lid() {
    translate([0,0,lid_height+wall]) {
	difference() {
	    union() {
		scale([1,1,-1]) lozenge_box(length+2*clearance+2*wall,width+2*clearance+2*wall,lid_height,radius, wall);
		translate([2*wall+clearance+10,2*wall+clearance+32,0]) clozenge(18,14,6,7);
	    }
	    translate([2*wall+clearance+10,2*wall+clearance+32,-wall-fuzz]) clozenge(14,10,6,5);
	    translate([2*wall+clearance+10,2*wall+clearance+32,wall]) clozenge(10,6,5,3);
	}
	difference() {
	    translate([0,10,0])
	    union() {
		translate([0,0,0]) cube([5,40,40]);
		translate([length+4*wall+2*clearance-5,0,0]) cube([5,40,40]);
	    }
	    rotate([25,0,0]) {
		translate([-1,0,5]) cube([length+4*wall+2*clearance+2,width*1.5,width]);
		translate([-1,20,-4]) cube([length+4*wall+2*clearance+2,width*1.5,3]);
	    }
	}
    }
    
}

module probe_cap() {
    difference() {
	union() {
	    translate([0,0,pcap_height+wall]) scale([1,1,-1]) lozenge_box(pcap_length, pcap_width,pcap_height,radius/2,wall);
	    translate([wall,wall+pcap_width,0]) rotate([0,0,-90]) scale([1,-1,1]) halflozenge(pcap_width,pcap_width-wall,10,radius);
	    translate([wall+pcap_length,wall+pcap_width,0]) rotate([0,0,-90]) halflozenge(pcap_width,pcap_width-wall,10,radius);
	}
	translate([wall+pcap_length/2-spike_spacing/2,wall+pcap_width/2,-fuzz]) cylinder(d=spike_dia, h=10+2*fuzz);
	translate([wall+pcap_length/2+spike_spacing/2,wall+pcap_width/2,-fuzz]) cylinder(d=spike_dia, h=10+2*fuzz);
	translate([wall/2,2*wall,-fuzz]) cube([pcap_length+wall,1.6,pcap_height]);
    }
}

module unit() {
    assembly();
    rotate([0,0,180]) translate([10,-width-2*wall-clearance,0]) lid();
    translate([length/2-pcap_length/2,-20,0]) probe_cap();
}


module grid(rows=2, cols=4, length=95, width=150) {
     for (i=[0:rows-1]) {
          for (j=[0:cols-1]) {
               translate([j*length, i*width,0]) unit();
          }
     }
}

grid();


    

