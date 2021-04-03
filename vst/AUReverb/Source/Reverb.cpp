#include "Reverb.hpp"

//void arg_error(){
//    LOG_ERROR("usage : ./reverb <input.wav>(required) [output.wav](optional)");
//    exit(-1);
//}
//
//float db_to_vol(float db){
//    return pow(10,db/20);
//}
//
//float vol_to_db(float a){
//    return 20*log10(a);
//}

//int main(int argc, char** argv){
//    LOG_INFO("--------------");
//    LOG_INFO("Moorer Reverb");
//    LOG_INFO("--------------");
//    std::string input_file;
//    std::string output_file;
//    if(argc<1){
//        arg_error();
//    }
//    else if(argc == 2){
//        input_file = argv[1];
//        output_file = "output.wav";
//    }
//    else if(argc == 3){
//        input_file = argv[1];
//        output_file = argv[2];
//    }else{
//        arg_error();
//    }
//    Wave w;
//    w.read_wav(argv[1]);
//    w.init_channels_maxmin();
//    w.update_channel_maxmin();
//
//    // calculate coefficients 
//    calc_g();
//    calc_coeff();
//    MoorerReverb mr;
//    mr.init();
//
//    // do processing here
//    for(int i=0; i<w.num_samples; ++i){
//        for(int c=0; c<w.num_channels;++c){
//            // wet dry mix biasing 
//            w.samples[c][i] = w.samples[c][i]*9.0f + mr.process(w.samples[c][i]);
//        }
//    }
//
//    // normalizing to db 
//    LOG_INFO("normalized_volume : "<<db_to_vol(-1.5f));
//    LOG_INFO("normalized_volume_db: "<<-1.5f);
//    w.update_channel_maxmin();
//    w.normalize(-db_to_vol(-1.5f),db_to_vol(-1.5f));
//    w.write_wav(output_file);
//    LOG_INFO("input_file : "<<input_file);
//    LOG_INFO("output_file : "<<output_file);
//    w.info_summary();
//    return 0;
//}


