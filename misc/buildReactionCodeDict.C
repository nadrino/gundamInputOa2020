
std::string reactionCodesPath = "./reactionCodes.txt";

std::map<int, int> colorOverrides{
     {1, 801}
    ,{-1, 801}
    ,{2, 634}
    ,{-2, 634}
    ,{11, 413}
    ,{-11, 413}
    ,{12, 843}
    ,{-12, 843}
    ,{13, 867}
    ,{-13, 867}
    ,{21, 593}
    ,{-21, 593}
    ,{26, 602}
    ,{-26, 602}
};

void buildReactionCodeDict(){

  std::vector<int> reactionCodesList;
  std::map<int, std::string> codesDict;
  std::map<int, int> colorsDict;
  std::vector<int> usedColors;


  auto lines = GenericToolbox::dumpFileAsVectorString(reactionCodesPath);
  for( auto& line : lines ){
    auto split = GenericToolbox::splitString(line, " : ");
    if( split.size() == 2 ){
      int code = std::stoi( GenericToolbox::trimString(split.at(0), " ") );
      codesDict[code] = GenericToolbox::trimString(split.at(1), " ");
      reactionCodesList.emplace_back(code);

      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "-->", "#rightarrow");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "ETA0", "#Eta_{0}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "LAMBDA", "#Lambda");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "GAMMA", "#gamma");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "LEPTON+", "l+");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "LEPTON-", "l-");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], " OR ", " or ");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "MULTI", "mult.");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "MESONS", "Mesons");

      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "NEUBAR", "#bar{#nu}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "NEU", "#nu");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "PI0", "#pi^{0}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "PI+", "#pi^{+}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "PI-", "#pi^{-}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "PI", "#pi");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "K0", "K^{0}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "K+", "K^{+}");
      GenericToolbox::replaceSubstringInsideInputString(codesDict[code], "K-", "K^{-}");

    }
  }

  std::cout << GenericToolbox::parseMapAsString(codesDict) << std::endl;

  int colorIndex{0};
  std::stringstream ss;
  ss << "# ReactionCode Dictionary" << std::endl;
  ss << "- { value: 0, title: \"UNDEFINED\", color: 1 } # kBlack" << std::endl;
  for( auto& reactionCode : reactionCodesList ){
    ss << "- { value: " << reactionCode
    << ", title: \"" << codesDict[reactionCode] << "\"";

    if( GenericToolbox::doesKeyIsInMap(reactionCode, colorOverrides) ){
      ss << ", color: " << colorOverrides[reactionCode];
      usedColors.emplace_back(colorOverrides[reactionCode]);
    }
    else{
      int colorCandidate;
      do{
        colorCandidate = GenericToolbox::defaultColorWheel[colorIndex++ % GenericToolbox::defaultColorWheel.size()];
        if( colorIndex >= GenericToolbox::defaultColorWheel.size() ){
          colorIndex = kGray;
          break;
        }
      } while(GenericToolbox::doesElementIsInVector(colorCandidate, usedColors));

      usedColors.emplace_back(colorCandidate);
      ss << ", color: " << colorCandidate;
    }

    if(reactionCode < 0) ss << ", fillStyle: " << 3001;

    ss << " }" << std::endl;
  }

  std::cout << ss.str() << std::endl;
  GenericToolbox::dumpStringInFile("ReactionCodesDict.yaml", ss.str());

}