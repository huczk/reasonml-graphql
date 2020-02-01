open ReactUtils;

[@react.component]
let make = (~page=1, ~pages=1, ~setPage=ignore) =>
  <nav className="pagination">
    {Belt.Array.range(1, pages)
     ->Belt.Array.map(p =>
         <li
           key={p->string_of_int}
           className={"page-item ng-scope " ++ (p == page ? "active" : "")}>
           <Link
             className="page-link ng-binding"
             href=""
             onClick={eventPrevent(_ => setPage(_ => p))}>
             {p->string_of_int->s}
           </Link>
         </li>
       )
     |> React.array}
  </nav>;