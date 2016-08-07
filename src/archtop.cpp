#include <wx/wx.h>
#include <memory>
#include <point.h>
#include <arc.h>
#include <drawing.h>
#include <parsedxf.h>
#include <dxflib/dl_dxf.h>

class Archtop: public wxApp {
    public:
        virtual bool OnInit();
};

class MainFrame: public wxFrame {
    public:
        MainFrame(wxString const& title, wxPoint const& pos, wxSize const& size);

    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnRun(wxCommandEvent& event);
        wxDECLARE_EVENT_TABLE();

        wxWindow* m_drawingWindow;
};

enum {
    ID_RUN = 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(Archtop);


bool Archtop::OnInit() {
    MainFrame * frame(new MainFrame("Archtop", wxPoint(200,200), wxSize(200, 200)));
    frame->Show(true);
    return true;
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
}

void MainFrame::OnRun(wxCommandEvent& event) {
    //calculate the max
    wxClientDC dc(m_drawingWindow);

    dc.SetBrush(wxNullBrush); //turn off fill
    wxPen myPen(*wxBLACK, 1);
    dc.SetPen(myPen);

    //dc.SetMapMode(wxMM_METRIC);
    dc.SetAxisOrientation(true, true);


    DL_Dxf dxf;
    ParseDXF<double> parser;
    dxf.in("test.dxf", &parser);

    wxSize dcSize(dc.GetSize()); //size of the 

    double minX = parser.GetDrawing().MinX();
    double minY = parser.GetDrawing().MinY();
    double maxX = parser.GetDrawing().MaxX();
    double maxY = parser.GetDrawing().MaxY();

    double xSpan = maxX;
    double ySpan = maxY;
    double drawingAspect = ySpan/xSpan;
    double screenAspect=(dcSize.y*1.0)/dcSize.x;
    double scale=dcSize.x/xSpan;

    if (scale*ySpan > dcSize.y) {
        scale = dcSize.y/ySpan;
    }

    std::cerr << "XScale " << (dcSize.x/xSpan) << " YSpan " << (dcSize.y/ySpan) << std::endl;

    std::cerr << "Drawing extents X=" << xSpan << " Y=" << ySpan << std::endl;
    std::cerr << "Window size " << dcSize.x << " , " << dcSize.y << std::endl;
    std::cerr << "Logical scale " << scale << std::endl;

    dc.SetDeviceOrigin(0, dcSize.y);

    parser.GetDrawing().Draw(dc, scale);

    std::pair<double, double> center = parser.GetDrawing().GetCenter();

    dc.CrossHair(center.first*scale, center.second*scale);

    Line<double> xOrig(Point<double>(0,0), Point<double>(100,0));
    Line<double> yOrig(Point<double>(0,0), Point<double>(0,100));
    xOrig.Draw(dc, scale);
    yOrig.Draw(dc, scale);
}

MainFrame::MainFrame(wxString const& title, wxPoint const& pos, wxSize const& size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , m_drawingWindow(NULL) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);


    //create drawing area
    m_drawingWindow = new wxWindow(this, 
                                   wxID_ANY, 
                                   wxDefaultPosition,
                                   wxSize(400,400),
                                   (wxBORDER_SIMPLE));
    m_drawingWindow->SetMinSize(wxSize(400,400));

    //create control buttons
    wxButton* runButton = new wxButton(this, ID_RUN, "Run", wxDefaultPosition, wxSize(-1, 26) );
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(runButton,
                      wxSizerFlags()
                       .FixedMinSize()
                       .Left());
    buttonSizer->AddStretchSpacer();


    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_drawingWindow, 
                   wxSizerFlags().Expand()
                                 .Top()
                                 .Left()
                                 .Proportion(1));
    mainSizer->Add(buttonSizer, 
                   wxSizerFlags()
                    .FixedMinSize()
                    .Bottom()
                    .Left()
                    .Border(wxALL, 10)
                    .Proportion(0));


    //add the main sizer to our frame
    SetSizerAndFit(mainSizer);


    CreateStatusBar();
    SetStatusText("Archtop");

    Connect(ID_RUN, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MainFrame::OnRun));
}

