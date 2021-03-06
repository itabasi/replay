#ifndef ROOT_TriFadcXscin
#define ROOT_TriFadcXscin

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriFadcXscin                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TClonesArray.h"
#include "THaNonTrackingDetector.h"
#include "Fadc250Module.h"

class THaScCalib;

class TriFadcXscin : public THaNonTrackingDetector {

public:
  TriFadcXscin( const char* name, 
	     const char* description = "",
	     Bool_t flip = kFALSE,
	     THaApparatus* a = NULL );
  virtual ~TriFadcXscin();

  virtual Int_t      Decode( const THaEvData& );
  virtual EStatus    Init( const TDatime& run_time );
  virtual Int_t      CoarseProcess( TClonesArray& tracks );
  virtual Int_t      FineProcess( TClonesArray& tracks );
  
  virtual Int_t      ApplyCorrections( void );

  Int_t GetNHits() const { return fNhit; }
  const Double_t* GetTimes() const { return fTime; }
  const Double_t* GetTuncer() const { return fdTime; }
  const Double_t* GetAmplitudes() const { return fAmpl; }
  const Double_t* GetXtime() const { return fXt; }
  const Double_t* GetXampl() const { return fXa; }
  
        Int_t GetNTracks() const { return fTrackProj->GetLast()+1; }
  const TClonesArray* GetTrackHits() const { return fTrackProj; }
  
  friend class THaScCalib;

  TriFadcXscin();  // for ROOT I/O
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

  Int_t    fNPED;        //number of samples included in FADC pedestal sum
  Int_t    fNSA;         //number of integrated samples after threshold crossing
  Int_t    fNSB;         //number of integrated samples before threshold crossing
  Int_t    fWin;         //total number of samples in FADC window
  Int_t    fTFlag;       //flag for FADC threshold on vs FADC threshold off

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
  

	// Per event data for s0 A-B strangness
  Double_t*   fUpT;         // [fNelem] Array of Left paddles TDC times (channels)
  Double_t*   fUpT_c;       // [fNelem] Array of Left PMT corrected TDC times (s)
  Double_t*   fDownT;         // [fNelem] Array of Right paddles TDC times (channels)
  Double_t*   fDownT_c;       // [fNelem] Array of Right PMT corrected TDC times (s)
  Double_t*   fUpA;         // [fNelem] Array of Left paddles ADC amplitudes
  Double_t*   fUpA_p;       // [fNelem] Array of Left paddles ADC minus ped values
  Double_t*   fUpA_c;       // [fNelem] Array of Left paddles corrected ADC ampl-s
  Double_t*   fDownA;         // [fNelem] Array of Right paddles ADC amplitudes
  Double_t*   fDownA_p;       // [fNelem] Array of Right paddles ADC minus ped values
  Double_t*   fDownA_c;       // [fNelem] Array of Right paddles corrected ADC ampl-s  
  
  Int_t      fNhit;       // Number of paddles with complete TDC hits (l&r)
  Int_t*     fHitPad;     // [fNhit] list of paddles with complete TDC hits

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

  Int_t* fLNhits;            //[fNelem] number of hit for each left pmt
  Int_t* fRNhits;	     //[fNelem] number of hit for each right pmt

  Decoder::Fadc250Module *fFADC;     //pointer to FADC250Module class


  // could be done on a per-hit basis instead
  Double_t*   fTime;       // [fNelem] corrected time for the paddle (s)
  Double_t*   fdTime;      // [fNelem] uncertainty in time (s)
  Double_t*   fAmpl;       // [fNelem] overall amplitude for the paddle
  Double_t*   fXt;         // [fNelem] x-position of hit in paddle from TDC (m)
  Double_t*   fXa;         // [fNelem] x-position of hit in paddle from ADC (m)
  
  TClonesArray*  fTrackProj;  // projection of track onto scintillator plane
                              // and estimated match to TOF paddle
  // Useful derived quantities
  double tan_angle, sin_angle, cos_angle;

  // Added by Barak (May 2016)
  Bool_t fFlip;
 
  static const char NDEST = 2;
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

  void           ClearEvent();
  void           DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );

  enum ESide { kLeft = 0, kRight = 1 };
  
  virtual  Double_t TimeWalkCorrection(const Int_t& paddle,
					   const ESide side);

  ClassDef(TriFadcXscin,0)   // Generic scintillator class
};

////////////////////////////////////////////////////////////////////////////////

#endif
