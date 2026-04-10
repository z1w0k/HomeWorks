#ifndef PAPER_REQUIREMENTS_H
#define PAPER_REQUIREMENTS_H
#include <string>

struct PaperRequirements {
    int minPublications;
    bool needConferenceParticipation;
    bool needLabResearch;
    std::string requiredLanguage;
    
    PaperRequirements(int pubs, bool needConf, bool needLab, const std::string& lang)
        : minPublications(pubs), needConferenceParticipation(needConf), 
          needLabResearch(needLab), requiredLanguage(lang) {}
};

#endif
