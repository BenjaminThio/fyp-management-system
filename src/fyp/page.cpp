#include <string>
#include <sstream>
#include <conio.h>
#include "json.h"
#include "database.h"
#include "uuid_v4.h"
#include "fyp/page.h"
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "renderer.h"
using namespace std;

namespace fyp {
    static int selected_option = 0;
    static int unit = 0;
    static json fyps;

    void start() {
        fyps = load("../data/fyp.json");
    }

    void push_frame(ostringstream& renderer) {
        switch (unit) {
            case static_cast<int>(Unit::LIST): {
                size_t counter = 0;

                for (auto& [key, val] : fyps.as_dictionary()) renderer << ((counter++ == selected_option) ? '>' : ' ') << ' ' << val["info"]["name"].parse_string() << endl;
                break;
            }
            case static_cast<int>(Unit::VIEW): {
                renderer << "Title: " << fyps[fyps.keys()[selected_option]]["info"]["name"].parse_string() << endl
                << endl
                << "Description: Testing123... You are gay :)";
                break;
            }
            default: {
                renderer << "FYP: Invalid Unit" << endl;
                break;
            }
        }
    }
        
    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP): {
                            if (selected_option - 1 >= 0) {
                                selected_option--;
                            } else {
                                selected_option = fyps.size() - 1;
                            }
                            break;
                        }
                        case static_cast<int>(Key::DOWN): {
                            if (selected_option + 1 < fyps.size()) {
                                selected_option++;
                            } else {
                                selected_option = 0;
                            }
                            break;
                        }
                    }

                    render_page();
                    play_sound("select");
                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    unit = static_cast<int>(Unit::VIEW);

                    render_page();
                    play_sound("select");
                    break;
                }
                case static_cast<int>(Key::ESCAPE): {
                    page = previous_page;

                    render_page();
                    play_sound("squeak");
                    break;
                }
            }
        }
    }

    void create_default_fyp_data() {
        json j = json::dictionary{};
        vector<string> titles = {
            "Developing of an IoT Based Smart Parking System",
            "Data hiding methods in High dynamic range (HDR) images",
            "Asset Management Mobile Application",
            "Malaysia Integrated EV Charging Information System",
            "Mobile-based Patient’s Healthcare Management System",
            "Combination of Generative Artificial Intelligence and Deep Reinforcement Learning: Performance Comparison",
            "Develop a customizable VR environment that supports the integration of eye tracking technology for paralyze patient",
            "Intelligent Home Design Hub",
            "A Mobile Application Development for Water Potability Prediction via Machine Learning Models",
            "Development of an IoT-Integrated App for Monitoring Hydroponic Farming Systems",
            "Arduino for Physics Electronics Experiments",
            "Web Application Development for Person Counting",
            "Stock Screening and Recommendation System for Singapore Real Estate Investment Trust",
            "Mining significant factors associated with defensive stocks from consumer products & services sector of Bursa Malaysia",
            "Real-Time Microplastic Detection in Marine Environments Using Edge AI",
            "Student Ride-Sharing Mobile Application for UTAR Sungai Long",
            "The use of transformer model to detect fraud in credit card transaction",
            "EcoTrack AI: A Personalized Carbon Footprint Tracker",
            "Sizing Tool for Solar PV and Battery Systems",
            "AI Personalized Tutor for University Students",
            "A Quantitative Approach on Sustainable Construction’s Blockchain Technology Awareness on Trust Issue and Change Resistance.",
            "On Self-Supervised Contrastive Learning via Neural Collapse",
            "Application of optimization techniques in portfolio management",
            "Mobile-based Location Services for Event Management",
            "Gamified Mood and Wellness Journal",
            "Intelligent Document Classification System for Automated Bank Loan Approval",
            "Web-based IT Service Management (ITSM) System for Service Request Management",
            "Course and University Recommendation System using Machine Learning",
            "AI-Powered Social Media Content Creator: Automating Engaging and Optimized Content Generation",
            "Smart Mobile Application for Resume Building and Career Advisory Services",
            "Price comparison website for online shopping",
            "Enhancing Diversity in Deepfake Detection Datasets",
            "LiDAR-based Freespace Detection and Path Planning for Autonomous Navigation",
            "A Web Application to Help Novice Players in PalWorld",
            "Self-Hosted Multi-Agent RAG System for Contextual Document Processing",
            "Text-Based Emotion Detection Using Deep Learning",
            "Automated Face Recognition System for Enhanced Security and Convenience at UTAR Entrances",
            "Phishing Website Detection Using Machine Learning Approaches",
            "Student Academic Performance Prediction System Using Data Mining Techniques",
            "Intelligent Waste Collection Monitoring System Using IoT and Data Analytics"
        };
        string owner_uuid = UUIDv4::generate();

        for (const string& title : titles) {
            string project_uuid = UUIDv4::generate();

            j[project_uuid] = json::dictionary{
                { "info", json::dictionary{
                        { "name", title },
                        { "category", "TESTING123" },
                        { "description", "TESTING456" },
                    }
                },
                { "isPublic", true },
                { "ownerUUID", owner_uuid },
                { "wishlistUUIDs", json::list{} }
            };
        }

        save("../../data/fyp.json", j);
    }
}
