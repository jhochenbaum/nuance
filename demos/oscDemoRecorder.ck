// launch with OSC_recv.ck

// host name and port
"localhost" => string hostname;
7000 => int port;

// get command line
if( me.args() ) me.arg(0) => hostname;
if( me.args() > 1 ) me.arg(1) => Std.atoi => port;

// send object
OscSend xmit;

// aim the transmitter
xmit.setHost( hostname, port );
0 => int ramp;
// infinite time loop

//spork ~ swell(0,1024,.1) @=> Shred s;
while( true )
{
   
    // start the message...
    // the type string ',f' expects a single float argument
    xmit.startMsg( "/nuance", "iffff" );
    
    //ramp => xmit.addInt;
    Std.rand2( 1, 1 ) => int temp2 => xmit.addInt;
    Std.rand2f( 0.5, .5 ) => float temp3 => xmit.addFloat;
    Std.rand2f( 0.5, 1.0 ) => float temp4 => xmit.addFloat;
    Std.rand2f( 0.5, 1.0 ) => float temp5 => xmit.addFloat;
    Std.rand2f( 0.5, 1.0 ) => float temp6 => xmit.addFloat;
    //temp => xmit.addInt;
    //<<< "sent (via OSC):", temp>>>;
    //<<<temp2, temp3, temp4>>>;
   // <<<temp2>>>;
    // advance time
    //.5::second => now;
    //.0125::second => now;
    7::samp => now;
    //ramp++;
    //<<<ramp>>>;
}

fun void swell(int begin, int end, int grain)
{
    while (true){
        //ramp up
        for (begin => int i; i < end; i + grain => i)
        {
            
            i => ramp;
            2::samp => now;
        }
        
        //ramp down
        for (end => int i; i > begin; i - grain => i)
        {
            
            i => ramp;
            2::samp => now;
        }  
    }
}
