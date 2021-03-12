#include "reverb.hpp"


// Recommended values from https://azrael.digipen.edu/MAT321/Moorer.pdf
// Also from Ken Steiglitz page 294

float delays_ms[] = {50.0f, 56.0f, 61.0f, 68.0f, 72.0f, 78.0f};

float g_25khz[] = {0.24f, 0.26f, 0.28f, 0.29f, 0.30f, 0.32f};
float g_50khz[] = {0.46f, 0.48f, 0.50f, 0.52f, 0.53f, 0.55f};

// float g_44_1khz[6];
float g_44_1khz[6] = {};
float lp_coeff[6] = {};

// lerp g from 25 and 50 khz vals in research paper 
void calc_g(){
    for(int i=0;i<6;++i){
        float m = (g_50khz[i] - g_25khz[i]) / 25000.0f;
        g_44_1khz[i] = m*(44100.0f - 25000.0f) + g_25khz[i];
        LOG_INFO("g("<<i<<") : "<<g_44_1khz[i]);
    }
    /*
        [INFO] g(0) : 0.40808
        [INFO] g(1) : 0.42808
        [INFO] g(2) : 0.44808
        [INFO] g(3) : 0.46572
        [INFO] g(4) : 0.47572
        [INFO] g(5) : 0.49572
    */
}

// normalized the comb gain to be 0.83 for low pass one pole coeff
// R/(1-g) = 0.83 
void calc_coeff(){
    for(int i=0;i<6;++i){
        lp_coeff[i] = (1.0f - g_44_1khz[i]) * 0.83f;
        LOG_INFO("lp_coeff("<<i<<") : "<<lp_coeff[i]);
    }
    /*
        [INFO] lp_coeff(0) : 0.491294
        [INFO] lp_coeff(1) : 0.474694
        [INFO] lp_coeff(2) : 0.458094
        [INFO] lp_coeff(3) : 0.443452
        [INFO] lp_coeff(4) : 0.435152
        [INFO] lp_coeff(5) : 0.418552
    */
}


class MoorerReverb{
public:
    CombFilter cf[6];
    AllPassFilter ap[1];

public:
    void init(){
        // number of combs
        int n_cf = sizeof(cf)/sizeof(cf[0]);
        LOG_INFO("n_combs : "<<n_cf);

        // set the comb filter parameters 
        float max_delay_ms = 100.0f;
        for(int i=0;i<n_cf;++i){
            cf[i].init(delays_ms[i],max_delay_ms,g_44_1khz[i],lp_coeff[i],44100);
            cf[i].delay_filter.set_delay_ms(delays_ms[i],44100);
        }

        // set all pass parameters - magic numbers from Mr Moorer
        ap[0].init(6.0f,20.0f,0.7f,44100);
        

    }
    float process(float n){
        float out = 0.0f;
        int n_cf = sizeof(cf)/sizeof(cf[0]);

        // accumulate from combs
        for(int i=0;i<n_cf;++i){
            // normalize for each comb to suppress total
            float cy = cf[i].process(n);
            out = out + cy;
        }

        //out = out + sum/6;

        // process through allpass 
        out = ap[0].process(out);

        return out;
    }
};

void arg_error(){
    LOG_ERROR("usage : ./reverb <input.wav>(required) [output.wav](optional)");
    exit(-1);
}

float db_to_vol(float db){
    return pow(10,db/20);
}

float vol_to_db(float a){
    return 20*log10(a);
}

int main(int argc, char** argv){
    LOG_INFO("--------------");
    LOG_INFO("Moorer Reverb");
    LOG_INFO("--------------");
    std::string input_file;
    std::string output_file;
    if(argc<1){
        arg_error();
    }
    else if(argc == 2){
        input_file = argv[1];
        output_file = "output.wav";
    }
    else if(argc == 3){
        input_file = argv[1];
        output_file = argv[2];
    }else{
        arg_error();
    }
    Wave w;
    w.read_wav(argv[1]);
    w.init_channels_maxmin();
    w.update_channel_maxmin();

    // calculate coefficients 
    calc_g();
    calc_coeff();
    MoorerReverb mr;
    mr.init();

    // do processing here
    for(int i=0; i<w.num_samples; ++i){
        for(int c=0; c<w.num_channels;++c){
            // wet dry mix biasing 
            w.samples[c][i] = w.samples[c][i]*9.0f + mr.process(w.samples[c][i]);
        }
    }

    // normalizing to db 
    LOG_INFO("normalized_volume : "<<db_to_vol(-1.5f));
    LOG_INFO("normalized_volume_db: "<<-1.5f);
    w.update_channel_maxmin();
    w.normalize(-db_to_vol(-1.5f),db_to_vol(-1.5f));
    w.write_wav(output_file);
    LOG_INFO("input_file : "<<input_file);
    LOG_INFO("output_file : "<<output_file);
    w.info_summary();
    return 0;
}


