#ifndef ROOT_TriFadcScinF1
#define ROOT_TriFadcScinF1

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriFadcScinF1                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaNonTrackingDetector.h"
#include "Fadc250Module.h"
#include "F1TDCModule.h"

class THaScCalib;
class TClonesArray;

class TriFadcScinF1 : public THaNonTrackingDetector {

public:
  TriFadcScinF1( const char* name, const char* description = "",
		   THaApparatus* a = NULL );
  TriFadcScinF1();
  virtual ~TriFadcScinF1();

  virtual void       Clear( Option_t* ="" );
  virtual Int_t      Decode( const THaEvData& );
  virtual EStatus    Init( const TDatime& run_time );
  virtual Int_t      CoarseProcess( TClonesArray& tracks );
  virtual Int_t      FineProcess( TClonesArray& tracks );

  virtual Int_t      ApplyCorrections( void );

  Int_t GetNHits() const { return fNhit; }
  const Double_t* GetTimes() const { return fTime; }
  const Double_t* GetTuncer() const { return fdTime; }
  const Double_t* GetAmplitudes() const { return fAmpl; }
  const Double_t* GetYtime() const { return fYt; }
  const Double_t* GetYampl() const { return fYa; }
  
  static const Int_t NDEST = 2;
  struct DataDest {
    Int_t*    nthit;
    Int_t*    nahit;
    Double_t*  tdc;
    Double_t*  tdc_c;
    Double_t*  adc;
    Double_t*  adc_p;
    Double_t*  adc_c;
    Double_t*  offset;
    Double_t*  ped;
    Double_t*  gain;
  } fDataDest[NDEST];     // Lookup table for decoder

  friend class THaScCalib;

  //  TriFadcScinF1();  // for ROOT I/O
protected:

  // Calibration
  Double_t*   fLOff;       // [fNelem] TDC offsets for left paddles
  Double_t*   fROff;       // [fNelem] TDC offsets for right paddles
  Double_t*   fLPed;       // [fNelem] ADC pedestals for left paddles
  Double_t*   fRPed;       // [fNelem] ADC pedestals for right paddles
  Double_t*   fLGain;      // [fNelem] ADC gains for left paddles
  Double_t*   fRGain;      // [fNelem] ADC gains for right paddles

  Double_t    fTdc2T;      // linear conversion between TDC and time (s/ch)
  Double_t    fCn;         // speed of light in material  (meters/second)

  Int_t       fNTWalkPar;  // number of timewalk correction parameters
  Double_t*   fTWalkPar;   // [fNTWalkPar] time walk correction parameters
  Double_t    fAdcMIP;     // nominal ADC above pedestal for MIP

  Double_t*   fTrigOff;     // [fNelem] Induced offset of trigger time from
                            // diff between trigger and retiming.
                            // Visible in coincidence data.

  Double_t    fAttenuation; // in m^-1: attenuation length of material
  Double_t    fResolution;  // average time resolution per PMT (s)

  Int_t       fNPED;        //number of samples included in FADC pedestal sum
  Int_t       fNSA;         //number of integrated samples after threshold crossing
  Int_t       fNSB;         //number of integrated samples before threshold crossing
  Int_t       fWin;         //total number of samples in FADC window
  Int_t       fTFlag;       //flag for FADC threshold on vs FADC threshold off

  // Per-event data
  Int_t       fLTNhit;     // Number of Left paddles TDC times
  Double_t*   fLT;         // [fNelem] Array of Left paddles TDC times (channels)
  Double_t*   fLT_c;       // [fNelem] Array of Left PMT corrected TDC times (s)
  Int_t       fRTNhit;     // Number of Right paddles TDC times
  Double_t*   fRT;         // [fNelem] Array of Right paddles TDC times (channels)
  Double_t*   fRT_c;       // [fNelem] Array of Right PMT corrected TDC times (s)
  Int_t       fLANhit;     // Number of Left paddles ADC amplitudes
  Double_t*   fLA;         // [fNelem] Array of Left paddles ADC amplitudes
  Double_t*   fLA_p;       // [fNelem] Array of Left paddles ADC minus ped values
  Double_t*   fLA_c;       // [fNelem] Array of Left paddles corrected ADC ampl-s
  Int_t       fRANhit;     // Number of Right paddles ADC amplitudes
  Double_t*   fRA;         // [fNelem] Array of Right paddles ADC amplitudes
  Double_t*   fRA_p;       // [fNelem] Array of Right paddles ADC minus ped values
  Double_t*   fRA_c;       // [fNelem] Array of Right paddles corrected ADC ampl-s


  Int_t      fNhit;       // Number of paddles with complete TDC hits (l&r)
  Int_t*     fHitPad;     // [fNhit] list of paddles with complete TDC hits

  // could be done on a per-hit basis instead
  Double_t*   fTime;       // [fNelem] corrected time for the paddle (s)
  Double_t*   fdTime;      // [fNelem] uncertainty in time (s)
  Double_t*   fAmpl;       // [fNelem] overall amplitude for the paddle
  Double_t*   fYt;         // [fNelem] y-position of hit in paddle from TDC (m)
  Double_t*   fYa;         // [fNelem] y-position of hit in paddle from ADC (m)

  //FADC
  Double_t*   fLPeak;        // [fNelem] Array of Left paddles FADC ADC peak value
  Double_t*   fLT_FADC;      // [fNelem] Array of Left paddles FADC TDC times (channels)
  Double_t*   fLT_FADC_c;    // [fNelem] Array of Left PMT corrected FADC TDC times (s)
  Int_t* floverflow;         //[fNelem] FADC overflowbit
  Int_t* flunderflow;        //[fNelem] FADC underflowbit
  Int_t* flpedq;             //[fNelem] FADC pedestal quality bit

  Double_t*   fRPeak;        // [fNelem] Array of right paddles FADC ADC peak value
  Double_t*   fRT_FADC;      // [fNelem] Array of right paddles FADC TDC times (channels)
  Double_t*   fRT_FADC_c;    // [fNelem] Array of right PMT corrected FADC TDC times (s)
  Int_t* froverflow;         //[fNelem] FADC overflowbit
  Int_t* frunderflow;        //[fNelem] FADC underflowbit
  Int_t* frpedq;             //[fNelem] FADC pedestal quality bit

  Int_t* fLNhits;           //[fNelem] number of hits for each Left PMT
  Int_t* fRNhits;           //[fNelem] number of hits for each Right PMT

  Decoder::Fadc250Module *fFADC;     //pointer to FADC250Module class



  void           DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );

  enum ESide { kLeft = 0, kRight = 1 };

  virtual  Double_t TimeWalkCorrection(const Int_t& paddle,
					   const ESide side);

  ClassDef(TriFadcScinF1,0)   // Generic scintillator class
};

////////////////////////////////////////////////////////////////////////////////

#endif
