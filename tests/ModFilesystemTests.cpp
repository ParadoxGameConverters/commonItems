//#include "../ModLoader/ModFilesystem.h"
//#include "../OSCompatibilityLayer.h"
//#include "gmock/gmock-matchers.h"
//#include "gtest/gtest.h"
//
//
//
//TEST(ModFilesystemTests, MissingFileReturnsNullopt)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/non_file.txt");
//	EXPECT_FALSE(file_path.has_value());
//}
//
//
//TEST(ModFilesystemTests, FileCanBeFoundInGameRoot)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/test_file.txt");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/game_root/test_folder/test_file.txt");
//}
//
//
//TEST(ModFilesystemTests, FileIsReplacedByMod)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/test_file.txt");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_one/test_folder/test_file.txt");
//}
//
//
//TEST(ModFilesystemTests, LatestModDeterminesFile)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/test_file.txt");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt");
//}
//
//
//TEST(ModFilesystemTests, ModDoesNotReplaceFileIfFileNotInMod)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const Mod mod_three("Mod Three", "ModFilesystem/GetActualFileLocation/mod_three");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two, mod_three});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/test_file.txt");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt");
//}
//
//
//TEST(ModFilesystemTests, ReplacePathBlocksEarlierInstancesOfFile)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const Mod mod_three("Mod Three", "ModFilesystem/GetActualFileLocation/mod_three", {}, {"test_folder"});
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two, mod_three});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/test_file.txt");
//	EXPECT_FALSE(file_path.has_value());
//}
//
//
//TEST(ModFilesystemTests, ReplacePathOnlyBlocksActualPath)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const Mod mod_three("Mod Three", "ModFilesystem/GetActualFileLocation/mod_three", {}, {"test_fold"});
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two, mod_three});
//
//	const auto file_path = mod_filesystem.GetActualFileLocation("test_folder/test_file.txt");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt");
//}
//
//
//TEST(ModFilesystemTests, MissingFolderReturnsNullopt)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	const auto file_path = mod_filesystem.GetActualFolderLocation("test_folder/non_folder");
//	EXPECT_FALSE(file_path.has_value());
//}
//
//
//TEST(ModFilesystemTests, FolderCanBeFoundInGameRoot)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	const auto file_path = mod_filesystem.GetActualFolderLocation("test_folder/deeper_folder");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/game_root/test_folder/deeper_folder");
//}
//
//
//TEST(ModFilesystemTests, FolderIsReplacedByMod)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one});
//
//	const auto file_path = mod_filesystem.GetActualFolderLocation("test_folder/deeper_folder");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_one/test_folder/deeper_folder");
//}
//
//
//TEST(ModFilesystemTests, LatestModDeterminesFolder)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	const auto file_path = mod_filesystem.GetActualFolderLocation("test_folder/deeper_folder");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_two/test_folder/deeper_folder");
//}
//
//
//TEST(ModFilesystemTests, ModDoesNotReplaceFolderIfFolderNotInMod)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const Mod mod_three("Mod Three", "ModFilesystem/GetActualFileLocation/mod_three");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two, mod_three});
//
//	const auto file_path = mod_filesystem.GetActualFolderLocation("test_folder/deeper_folder");
//	ASSERT_TRUE(file_path.has_value());
//	EXPECT_EQ(*file_path, "ModFilesystem/GetActualFileLocation/mod_two/test_folder/deeper_folder");
//}
//
//
//TEST(ModFilesystemTests, ReplacePathBlocksEarlierInstancesOfFolder)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const Mod mod_three("Mod Three", "ModFilesystem/GetActualFileLocation/mod_three", {}, {"test_folder/"});
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two, mod_three});
//
//	const auto file_path = mod_filesystem.GetActualFolderLocation("test_folder/deeper_folder");
//	EXPECT_FALSE(file_path.has_value());
//}
//
//
//TEST(ModFilesystemTests, NoFilesInMissingDirectory)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolder("/non_folder"), testing::UnorderedElementsAre());
//}
//
//
//TEST(ModFilesystemTests, FilesInGameRootAreFound)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolder("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/game_root/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/root_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, ModFilesAddToAndReplaceGameRootFiles)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolder("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_one/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/root_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, ModFilesAddToAndReplaceEarlierModFiles)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolder("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/root_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, ReplaceFolderKeepsFilesFromBeingFound)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two", {}, {"test_folder"});
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolder("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, NoFoldersInMissingDirectory)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	EXPECT_THAT(mod_filesystem.GetAllSubfolders("/non_folder"), testing::UnorderedElementsAre());
//}
//
//
//TEST(ModFilesystemTests, FoldersInGameRootAreFound)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	EXPECT_THAT(mod_filesystem.GetAllSubfolders("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/game_root/test_folder/deeper_folder",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/game_folder"));
//}
//
//
//TEST(ModFilesystemTests, ModFoldersAddToAndReplaceGameRootFolders)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one});
//
//	EXPECT_THAT(mod_filesystem.GetAllSubfolders("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_one/test_folder/deeper_folder",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/game_folder",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_folder"));
//}
//
//
//TEST(ModFilesystemTests, ModFoldersAddToAndReplaceEarlierModFolders)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	EXPECT_THAT(mod_filesystem.GetAllSubfolders("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_two/test_folder/deeper_folder",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/game_folder",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_folder",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_folder"));
//}
//
//
//TEST(ModFilesystemTests, ReplaceFolderKeepsFoldersFromBeingFound)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two", {}, {"test_folder"});
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	EXPECT_THAT(mod_filesystem.GetAllSubfolders("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_two/test_folder/deeper_folder",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_folder"));
//}
//
//
//TEST(ModFilesystemTests, NoFilesInMissingDirectoryTree)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolderRecursive("/non_folder"), testing::UnorderedElementsAre());
//}
//
//
//TEST(ModFilesystemTests, FilesInGameRootAndSubfoldersAreFound)
//{
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolderRecursive("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/game_root/test_folder/deeper_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/game_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/root_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, ModFilesAndSubfoldersAddToAndReplaceGameRootFiles)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolderRecursive("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_one/test_folder/deeper_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/game_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/root_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, ModFilesAndSubfoldersAddToAndReplaceEarlierModFiles)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two");
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolderRecursive("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_two/test_folder/deeper_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/game_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/game_root/test_folder/root_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_one/test_folder/mod_one_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_file.txt"));
//}
//
//
//TEST(ModFilesystemTests, ReplaceFolderKeepsFilesAndSubfoldersFromBeingFound)
//{
//	const Mod mod_one("Mod One", "ModFilesystem/GetActualFileLocation/mod_one");
//	const Mod mod_two("Mod Two", "ModFilesystem/GetActualFileLocation/mod_two", {}, {"test_folder"});
//	const commonItems::ModFilesystem mod_filesystem("ModFilesystem/GetActualFileLocation/game_root", {mod_one, mod_two});
//
//	EXPECT_THAT(mod_filesystem.GetAllFilesInFolderRecursive("test_folder"),
//		 testing::UnorderedElementsAre("ModFilesystem/GetActualFileLocation/mod_two/test_folder/deeper_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_folder/dummy.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/test_file.txt",
//			  "ModFilesystem/GetActualFileLocation/mod_two/test_folder/mod_two_file.txt"));
//}