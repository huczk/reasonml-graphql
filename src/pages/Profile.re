open ReactUtils;

type tabs =
  | Author(string)
  | Favorited(string);

let listType =
  fun
  | Author(username) => ArticleList.Author(username)
  | Favorited(username) => ArticleList.Favorited(username);

[@react.component]
let make = (~username, ~favorites=false) => {
  let isLoggedUser = UserProvider.useUser().user->UserProvider.isLoggedUser;

  let (tab, setTab) =
    React.useState(() => favorites ? Favorited(username) : Author(username));
  let (simple, _full) =
    ApolloHooks.useQuery(
      ~variables=Profile_Query.GetProfile.makeVariables(~username, ()),
      Profile_Query.GetProfile.definition,
    );

  <div className="profile-page">
    {switch (simple) {
     | Data(data) =>
       switch (data##profile) {
       | Some(profile) =>
         <>
           <div className="user-info">
             <div className="container">
               <div className="row">
                 <div className="col-xs-12 col-md-10 offset-md-1">
                   <img src=profile##image className="user-img" />
                   <h4> {profile##username->s} </h4>
                   <p> {profile##bio->s} </p>
                   {isLoggedUser(profile##id)
                      ? <Link
                          className="btn btn-sm btn-outline-secondary action-btn"
                          href="#/settings">
                          <i className="ion-gear-a" />
                          "Edit Profile Settings"->s
                        </Link>
                      : <FollowButton
                          id=profile##id
                          username=profile##username
                          following=profile##following
                          className="action-btn"
                        />}
                 </div>
               </div>
             </div>
           </div>
           <div className="container">
             <div className="row">
               <div className="col-xs-12 col-md-10 offset-md-1">
                 <div className="articles-toggle">
                   <ul className="nav nav-pills outline-active">
                     <TabItem
                       text="My Articles"
                       active={tab == Author(username)}
                       onClick={eventPrevent(_ => {
                         ReasonReactRouter.replace("/#/profile/" ++ username);
                         setTab(_ => Author(username));
                       })}
                     />
                     <TabItem
                       text="Favorited Articles"
                       active={tab == Favorited(username)}
                       onClick={eventPrevent(_ => {
                         ReasonReactRouter.replace(
                           "/#/profile/" ++ username ++ "/favorites",
                         );
                         setTab(_ => Favorited(username));
                       })}
                     />
                   </ul>
                 </div>
                 <ArticleList listType={listType(tab)} />
               </div>
             </div>
           </div>
         </>
       | None => React.null
       }
     | _ => React.null
     }}
  </div>;
};