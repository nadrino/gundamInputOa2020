

double lowBound = -1;
double range = 2;
TF1* f;
TCanvas *c1 = new TCanvas("c1");

ENUM_EXPANDER(
  DialType, -1
  ,Invalid
  ,Normalization // response = dial
  ,Spline        // response = spline(dial)
  ,Graph         // response = graphInterpol(dial)
)

void mirroringX(){
  f = new TF1("f",[](double* x_, double* par_){
    double out;


    out = std::abs(std::fmod(
          x_[0] - lowBound,
          2*range
        ));

    if( out > range ){
      // odd pattern  -> mirrored -> decreasing effective X while increasing parameter (vice versa)
      out -= 2*range;
      out = -out;
    }

    out += lowBound;

    return out;
  },
  lowBound - range*2,
  lowBound + range*3
  );
  f->Draw();
  f->SetTitle(Form("Dial mirrored around %f < x < %f", lowBound, lowBound+range));
  f->GetXaxis()->SetTitle("Parameter value");
  f->GetYaxis()->SetTitle("Dial eval parameter");
  f->SetNpx(1000);
  c1->Modified();
}
